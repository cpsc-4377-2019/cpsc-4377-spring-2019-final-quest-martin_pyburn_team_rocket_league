#ifndef INTEGRITY_H
#define INTEGRITY_H

#include<string>
#include<map>
#include<iostream>
#include "Component.h"
#include "SDL.h"

class ObjectFactory;
class GraphicsDevice;
struct ObjectParams;

class Integrity : public Component
{
public:
	Integrity(std::shared_ptr<GameObject>);
	~Integrity();
	bool initialize(ObjectFactory*, GraphicsDevice*, ObjectParams*);
	void start();
	std::shared_ptr<GameObject> update();
	void finish();

	void damage(float);
	
	float integrity{ 0 };
private:

	bool dead{ false };
	ObjectFactory* factory;
	GraphicsDevice* gDevice;
	std::shared_ptr<GameObject> spawnEmitter();
};


#endif // !INTEGRITY_H