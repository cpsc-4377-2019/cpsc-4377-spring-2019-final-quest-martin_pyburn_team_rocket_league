#include "Library.h"
#include "tinyxml2.h"

#include <iostream>

using namespace std;
using namespace tinyxml2;

bool SaveGame(vector<Object*> objects, string filename) {
	XMLDocument doc;
	XMLDeclaration * dec = doc.NewDeclaration("xml version=\"1.0\" encoding=\"utf-8\"");
	doc.InsertFirstChild(dec);
	XMLNode * pRoot = doc.NewElement("Level");
	doc.InsertEndChild(pRoot);

	vector<Object*>::iterator objectsIter;
	for (objectsIter = objects.begin(); objectsIter != objects.end(); objectsIter++) {

		// Create Object element
		XMLElement * oElement = doc.NewElement("Object");

		// Create Body element
		XMLElement * bElement = doc.NewElement("Body");

		// Create the root element
		pRoot->InsertEndChild(oElement);

		// Get position of object
		Position2D pos = (*objectsIter)->getPosition();

		// Store the object position as xml
		bElement->SetAttribute("x", pos.x);
		bElement->SetAttribute("y", pos.y);
		bElement->SetAttribute("angle", pos.angle);

		// Insert the Body element
		oElement->InsertFirstChild(bElement);

		// Apply the correct type attribute
		switch ((*objectsIter)->objectType) {
		case objectTypes::SHIP:
			// Save Ship data
			oElement->SetAttribute("type", "Ship");
			// Add any Ship-specific data
			break;
		case objectTypes::ENEMY:
			// Save Enemy data
			oElement->SetAttribute("type", "Enemy");
			// Add any Enemy-specific data
			break;
		case objectTypes::PLANETOID:
			// Save Planetoid data
			oElement->SetAttribute("type", "Planetoid");
			// Add any Planetoid-specific data
			break;
		case objectTypes::MINERAL:
			// Save Mineral data
			oElement->SetAttribute("type", "Mineral");
			// Add any Mineral-specific data
			break;
		}
		XMLError eResult = doc.SaveFile((filename + ".xml").c_str());
		if (eResult != XML_SUCCESS) { printf("Error: %i\n", eResult); return eResult; }
	}

	return false;
}