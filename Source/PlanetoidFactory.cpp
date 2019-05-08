#include "PlanetoidFactory.h"
#include "Library.h"

PlanetoidFactory::PlanetoidFactory(Library * lib)
	: ObjectFactory(lib)
{
}

Object * PlanetoidFactory::create(tinyxml2::XMLElement * objectElement) {

	/****************************
		objectElement is the
		level xml file which
		points to dataset xml
		file and spritesheet
	****************************/

	// additional modification to the Object before returning
	tinyxml2::XMLElement* set = objectElement->FirstChildElement("setData");


	/*********************************
	BATCH PARSING OF PLANETOID DATASET
		Generates nested planetoids
	*********************************/

	// build path to the xml file for the planetoid
	const char* xmlFileName;
	set->QueryStringAttribute("xml", &xmlFileName);
	string xmlPath = "./Assets/Config/";
	xmlPath += xmlFileName;

	// create XMLDocument and load the file
	tinyxml2::XMLDocument planetoidDoc;
	if (planetoidDoc.LoadFile(xmlPath.c_str()) != tinyxml2::XML_SUCCESS) {
		printf("Bad File Path");
		exit(1);
	}

	// get the root
	tinyxml2::XMLElement* root = planetoidDoc.FirstChildElement("Planetoid");

	// get the number of planetoids in the set
	int count_in;
	root->QueryIntAttribute("count", &count_in);

	// array to store the Planetoids
	vector<shared_ptr<Planetoid >> PlanetoidArray;

	// array to store children's idices
	vector<const char*> ChildLists( count_in, "" );

	// itterate through the Planetoids
	for (tinyxml2::XMLElement* obj = root->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		PlanetoidParams params;
		
		obj->QueryFloatAttribute("area", &params.area);
		obj->QueryIntAttribute("layer", &params.layer);
		obj->QueryBoolAttribute("shell", &params.shell);
		obj->QueryIntAttribute("index", &params.index);
		
		const char* childlist = "";
		if ( obj->QueryStringAttribute( "children", &childlist ) == tinyxml2::XML_SUCCESS )
			ChildLists[params.index] = childlist;

		// Get Planetoid with values from xml
		shared_ptr<Planetoid> newPlanetoid = make_shared<Planetoid>( params );

		// apply common object attributes
		ObjectFactory::applyXMLAttributes(obj, newPlanetoid.get());

		// place each planetoid object in its index position
		PlanetoidArray.assign( params.index + 1, newPlanetoid );
	}

	// Find the top-level planetoid to be returned
	shared_ptr<Planetoid> bigPlanetoid = PlanetoidArray[ count_in - 1 ];

	ObjectFactory::applyXMLAttributes(objectElement, bigPlanetoid.get(), true );

	/*********************************
		parse the image file location
		from the dataset
	*********************************/

	// load texture file
	const char* imgFileName;
	set->QueryStringAttribute("img", &imgFileName);
	string imgPath = "./Assets/Images/";
	imgPath += imgFileName;

	shared_ptr<Texture> img = library->getArtAsset( imgPath );

	// Call recursive method to attach children to parents
	attachChildren( bigPlanetoid, ChildLists, PlanetoidArray, PlanetoidArray.size() - 1, img );

	return bigPlanetoid.get();
}

shared_ptr<Planetoid> PlanetoidFactory::attachChildren(
	shared_ptr<Planetoid> Parent,
	vector<const char*> ChildLists,
	vector<shared_ptr<Planetoid>> PlanetoidArray,
	int index,
	shared_ptr<Texture> img
)
{
	// attach the Texture
	Parent.get()->attachImage(img);

	size_t pos = 0;
	// get the list of children for the current parent
	string childlist = ChildLists[ index ];

	//itterate through the child list
	while ( ( pos = childlist.find(' ') ) != string::npos ) {
		// get the integer value of each entry
		int ni = stoi(childlist.substr( 0, pos ) );
		// append the current child to the parent
		Parent.get()->appendChild(
			// recursively append the children to each generation
			attachChildren( PlanetoidArray[ ni ], ChildLists, PlanetoidArray, ni, img )
		);
		// move pos to the next entry
		childlist.erase( 0, pos + 1 );
	}

	return Parent;
}