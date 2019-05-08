#ifndef CHASECOMPONENT_H
#define CHASECOMPONENT_H
#include "ObjectDefinitions.h"
#include "Definitions.h"
#include "RidgidBodyComponent.h"
#include "Component.h"

class Engine;

class Chase : public Component
{
public:

	Chase(std::shared_ptr<GameObject>);
	~Chase();
	void initialize(ObjectTemplate* temp);
	void start();
	std::shared_ptr<GameObject> update();
	void finish();

	eFloat distance{ 0.f };

private:

	eFloat speed{ 5.f };
	eFloat speedLimit{ 90.f };

};




#endif // CHASECOMPONENT_H
