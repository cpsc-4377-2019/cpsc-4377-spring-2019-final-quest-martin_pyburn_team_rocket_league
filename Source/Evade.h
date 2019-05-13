#ifndef EVADE_H
#define EVADE_H

#include "ObjectDefinitions.h"
#include "Definitions.h"
#include "RidgidBodyComponent.h"
#include "Component.h"
#include <random>

class Engine;

class Evade:public Component
{
public:
	Evade(std::shared_ptr<GameObject>);
	~Evade();

	void initialize(shared_ptr<resource_map> resources, ObjectTemplate* temp);

	void start();
	void finish();
	std::shared_ptr<GameObject> update();

	eFloat distance{ 9999.f };

private:
	eFloat speed{ 6.f };
	eFloat speedLimit{ 60.f };

	
};

#endif // !EVADE_H
