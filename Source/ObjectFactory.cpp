#include "Engine.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "TextureLibrary.h"
#include "PhysicsDevice.h"
#include "GraphicsDevice.h"
#include "SoundDevice.h"
#include "SpriteComponent.h"
#include "RidgidBodyComponent.h"
#include "ObjectListComponent.h"
#include "IntegrityComponent.h"
#include "FragBehavior.h"
#include "PeaShooter.h"
#include "MissileLauncher.h"
#include "Expiration.h"
#include "InputHandler.h"
#include "UserInputComponent.h"
#include "Accelerate.h"
#include "ChaseBehavior.h"
#include "Evade.h"
#include "Steering.h"
#include "PowerUpComponent.h"
#include "AutoFireComponent.h"
#include "ForceFieldComponent.h"
#include "PowerupFactory.h"
#include "Drone.h"
#include <algorithm>
#include <sstream>
#include <iterator>
#include <string>


ObjectFactory::ObjectFactory(shared_ptr<resource_map> resources) {
	this->resources = resources;
}

ObjectFactory::~ObjectFactory() {
}

void ObjectFactory::applyXML(XMLElement* ObjectXML, vector<shared_ptr<GameObject>>* objects)
{
	// allocate new GameObject
	int at = objects->size();
	shared_ptr<GameObject> object = make_shared<GameObject>();
	objects->push_back(object);

	// assign type
	const char* typeChar;
	if (ObjectXML->QueryStringAttribute("type", &typeChar) == XML_SUCCESS)
		object->type = objectTypesIndex.find(typeChar)->second;
	else object->type = objectTypes::NA;

	// store named objects
	const char* nameChar;
	if (ObjectXML->QueryStringAttribute("name", &nameChar) == XML_SUCCESS)
		namedObjects[nameChar] = object;

	// assign aggro
	const char* aggroName;
	if (ObjectXML->QueryStringAttribute("aggro", &aggroName) == XML_SUCCESS)
		object->aggro = namedObjects[aggroName];

	// assess danger
	float damage;
	if(ObjectXML->QueryFloatAttribute("damage", &damage) == XML_SUCCESS)
		object->damage = damage;

	shared_ptr<ObjectTemplate> temp = make_shared<ObjectTemplate>();

	// apply xml from current element
	applyXMLToTemplate(ObjectXML, temp.get());

	// check for external xml reference
	const char* external;
	XMLDocument doc;
	if (ObjectXML->QueryStringAttribute("external", &external) == XML_SUCCESS) {
		// load external xml file
		string externalPath = resources->xmlPath + external;
		if (doc.LoadFile(externalPath.c_str()) != tinyxml2::XML_SUCCESS) {
			printf("Bad File Path: %s", externalPath.c_str());
		}
		else {
			ObjectXML = doc.FirstChildElement();
			// apply external xml
			applyXMLToTemplate(ObjectXML, temp.get());
		}
	}

	// attach children
	getListFromXML(ObjectXML, objects, object);

	applyTemplate(temp, object);
}

void ObjectFactory::applyTemplate(shared_ptr<ObjectTemplate> temp, shared_ptr<GameObject> target)
{
	if ((*temp)[BODY]->set) attachComponent(BODY, target, temp.get());
	if ((*temp)[SPRITE]->set) attachComponent(SPRITE, target, temp.get());
	if ((*temp)[USERINPUT]->set) attachComponent(USERINPUT, target, temp.get());
	if ((*temp)[FRAGBEHAVIOR]->set) {
		shared_ptr<Frag> newFrag = make_shared<Frag>(target);
		newFrag->initialize(resources, temp.get());
		target->addComponent(newFrag);
	}
	if (target->type != objectTypes::POWERUP) {
		for (int c = 0; c < objectComponents::END; c++) {
			objectComponents comp = (objectComponents)c;
			if (comp != BODY && comp != SPRITE && comp != USERINPUT && comp != FRAGBEHAVIOR) {
				if((*temp)[comp]->set) attachComponent(comp, target, temp.get());
			}
		}
	}
	else {
		shared_ptr<PowerUp> newPowerUp = make_shared<PowerUp>(target);
		newPowerUp->initialize(this, temp);
		target->addComponent(dynamic_pointer_cast<Component>(newPowerUp));
	}
}

void ObjectFactory::attachComponent(objectComponents comptype, shared_ptr<GameObject> target, ObjectTemplate* temp)
{
	switch (comptype) {
		case BODY: target->addComponent(make_shared<RidgidBody>(target))->initialize(resources, temp); break;
		case SPRITE: target->addComponent(make_shared<Sprite>(target))->initialize(resources, temp); break;
		case USERINPUT: target->addComponent(make_shared<UserInput>(target))->initialize(resources, temp); break;
		case CHASEBEHAVIOR: target->addComponent(make_shared<Chase>(target))->initialize(resources, temp); break;
		case EVADEBEHAVIOR: target->addComponent(make_shared<Evade>(target))->initialize(resources, temp); break;
		case STEERBEHAVIOR: target->addComponent(make_shared<Steering>(target))->initialize(resources, temp); break;
		case EXPIRE: target->addComponent(make_shared<Expire>(target))->initialize(resources, temp); break;
		case INTEGRITY: target->addComponent(make_shared<Integrity>(target))->initialize(resources, temp); break;
		case FRAGBEHAVIOR: target->addComponent(make_shared<Frag>(target))->initialize(resources, temp); break;
		case PEASHOOTER: target->addComponent(make_shared<PeaShooter>(target))->initialize(resources, temp); break;
		case MISSILELAUNCHER: target->addComponent(make_shared<MissileLauncher>(target))->initialize(resources, temp); break;
		case ACCELERATE: target->addComponent(make_shared<Accelerate>(target))->initialize(resources, temp); break;
		case DRONE: target->addComponent(make_shared<Drone>(target))->initialize(resources, temp); break;
		case AUTOFIRE: target->addComponent(make_shared<AutoFire>(target))->initialize(resources, temp); break;
		case FORCEFIELD: target->addComponent(make_shared<ForceField>(target))->initialize(resources, temp); break;
	}
}

void ObjectFactory::applyXMLToTemplate(XMLElement* ObjectXML, ObjectTemplate* temp)
{
	// iterate through components
	for (int c = 0; c < objectComponents::END; c++) {

		// get the string value that corresponds to each component
		string compname = objectComponentsIndex.at(c);

		// get the xml element for the current component, if it exists
		XMLElement* comp = ObjectXML->FirstChildElement(compname.c_str());
		if (comp != nullptr) {

			// skip compnents with no attributes
			if (find(muted_components.begin(), muted_components.end(), c) != muted_components.end()) {
				(*temp)[c]->set = true;
				continue;
			}
			ObjectParams* newParams = (*temp)[c].get();
			comp->QueryFloatAttribute("x", &newParams->x);
			comp->QueryFloatAttribute("y", &newParams->y);
			comp->QueryFloatAttribute("a", &newParams->angle);
			comp->QueryFloatAttribute("vx", &newParams->vx);
			comp->QueryFloatAttribute("vy", &newParams->vy);
			comp->QueryFloatAttribute("va", &newParams->vangle);
			comp->QueryFloatAttribute("cx", &newParams->cx);
			comp->QueryFloatAttribute("cy", &newParams->cy);
			comp->QueryFloatAttribute("sx", &newParams->sx);
			comp->QueryFloatAttribute("sy", &newParams->sy);
			comp->QueryIntAttribute("sw", &newParams->sw);
			comp->QueryIntAttribute("sh", &newParams->sh);
			comp->QueryFloatAttribute("value", &newParams->value);
			comp->QueryFloatAttribute("density", &newParams->density);
			comp->QueryIntAttribute("radius", &newParams->radius);
			comp->QueryIntAttribute("layer", &newParams->layer);
			comp->QueryBoolAttribute("shell", &newParams->shell);
			// get path
			const char* pathName;
			if (comp->QueryStringAttribute("path", &pathName) == XML_SUCCESS)
				newParams->path = pathName;
			// get color
			const char* colorName;
			if (comp->QueryStringAttribute("color", &colorName) == XML_SUCCESS) {
				newParams->color = stoul(colorName, nullptr, 16);
				newParams->colorSet = true;
			}
			// parse points from Body element
			const char* pointString;
			if (comp->QueryStringAttribute("points", &pointString) == XML_SUCCESS) {
				istringstream iss(pointString);
				vector<string> ps{
						istream_iterator<string>{iss},
						istream_iterator<string>{}
				};
				for (int s = 0; s < ps.size(); s++)
					newParams->points.push_back(stof(ps.at(s)));
			}
			newParams->set = true;
		}
	}
}

void ObjectFactory::getListFromXML(XMLElement* ObjectXML, vector<shared_ptr<GameObject>>* objects, shared_ptr<GameObject> object)
{
	// build list of children, if there are any
	XMLElement* ChildrenXML = ObjectXML->FirstChildElement("Children");
	if (ChildrenXML != nullptr) {
		// attach ObjectList component
		shared_ptr<ObjectList> comp = make_shared<ObjectList>(object);
		comp->initialize(nullptr, nullptr);
		object->addComponent(dynamic_pointer_cast<Component>(comp));

		for (tinyxml2::XMLElement* obj = ChildrenXML->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement()) {
			int at = objects->size();
			obj->SetAttribute("type", "Planetoid");
			applyXML(obj, objects);
			comp->list.push_back(objects->at(at));
		}
	}
}

std::shared_ptr<GameObject> ObjectFactory::getPower()
{
	return power->getPower(resources->engine->getShip());
}
