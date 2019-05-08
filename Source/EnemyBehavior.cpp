#include"EnemyBehavior.h"
#include"GameObject.h"

EnemyBehavior::EnemyBehavior(GameObject* owner) :Component(owner)
{
	DistanceSelector*slector = new DistanceSelector(owner);

	node = slector;
}

EnemyBehavior::~EnemyBehavior()
{

}

void EnemyBehavior::initialize(ObjectTemplate * temp)
{
}

void EnemyBehavior::start()
{

}

void EnemyBehavior::finish()
{

}

shared_ptr<GameObject> EnemyBehavior::update()
{
	node->update();
}