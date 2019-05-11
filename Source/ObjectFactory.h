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
class PowerUpFactory;

class ObjectFactory {
public:
	ObjectFactory( Engine* engine, SDL_Renderer*, InputHandler*, TextureLibrary*, PhysicsDevice*, GraphicsDevice*);
	~ObjectFactory();

	void applyXML(XMLElement*, vector<shared_ptr<GameObject>>*);
	void applyTemplate(shared_ptr<ObjectTemplate>, shared_ptr<GameObject>);
	void applyXMLToTemplate(XMLElement*, ObjectTemplate*);

	void getListFromXML(XMLElement*, vector<shared_ptr<GameObject>>* objects, std::shared_ptr<GameObject>);
	
	std::shared_ptr<GameObject> getPower();

	string xmlPath = "";

	map<string, shared_ptr<GameObject>> namedObjects;

private:
	Engine* engine;
	InputHandler* input;
	TextureLibrary* library;
	PhysicsDevice* physics;
	GraphicsDevice* graphics;
	SDL_Renderer* renderer;
	unique_ptr<PowerUpFactory> power = make_unique<PowerUpFactory>(this);
};


#endif
