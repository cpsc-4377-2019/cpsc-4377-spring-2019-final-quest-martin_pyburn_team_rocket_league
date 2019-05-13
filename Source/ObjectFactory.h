#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include<string>
#include<map>
#include"ObjectDefinitions.h"

#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;

class Engine;
class GameObject;
class InputHandler;
class TextureLibrary;
struct SDL_Renderer;
class PhysicsDevice;
class GraphicsDevice;
class SoundDevice;
class PowerUpFactory;
class Component;

class ObjectFactory {
public:
	ObjectFactory(shared_ptr<resource_map> resources);
	~ObjectFactory();

	void applyXML(XMLElement*, vector<shared_ptr<GameObject>>*);
	void applyTemplate(shared_ptr<ObjectTemplate>, shared_ptr<GameObject>);
	void applyXMLToTemplate(XMLElement*, ObjectTemplate*);
	void attachComponent(objectComponents comptype, shared_ptr<GameObject> target, ObjectTemplate* temp);

	void getListFromXML(XMLElement*, vector<shared_ptr<GameObject>>* objects, std::shared_ptr<GameObject>);
	
	std::shared_ptr<GameObject> getPower();

	string xmlPath = "";

	map<string, shared_ptr<GameObject>> namedObjects;

private:
	std::shared_ptr<resource_map> resources;
	unique_ptr<PowerUpFactory> power = make_unique<PowerUpFactory>(this);
};


#endif
