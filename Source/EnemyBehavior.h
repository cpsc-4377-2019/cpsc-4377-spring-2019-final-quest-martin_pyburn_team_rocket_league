#ifndef ENEMYBEHAVIOR_H
#define ENEMYBEHAVIOR_H
#include "ObjectDefinitions.h"
#include "Definitions.h"
#include "RidgidBodyComponent.h"
#include "Component.h"
#include "DistanceSelectorNode.h"
class EnemyBehavior :public Component
{
public:
	EnemyBehavior(GameObject*);
	~EnemyBehavior();
	void initialize(ObjectTemplate* temp);
	void start();
	std::shared_ptr<GameObject> update();
	void finish();
private:
	DistanceSelector* node = nullptr;
};

#endif