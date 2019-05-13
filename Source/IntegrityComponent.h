#ifndef INTEGRITY_H
#define INTEGRITY_H

#include<string>
#include<map>
#include<iostream>
#include "Component.h"
#include "Definitions.h"
#include "SDL.h"

class ObjectFactory;
class GraphicsDevice;
class SoundDevice;
struct ObjectParams;

class Integrity : public Component
{
public:
	Integrity(std::shared_ptr<GameObject>);
	~Integrity();
	void initialize(shared_ptr<resource_map> resources, ObjectTemplate* temp);
	void start();
	std::shared_ptr<GameObject> update();
	void finish();

	void damage(float);
	
	float integrity{ 0 };
private:

	bool dead{ false };
	shared_ptr<resource_map> resources;
	std::shared_ptr<GameObject> spawnEmitter();
};


#endif // !INTEGRITY_H