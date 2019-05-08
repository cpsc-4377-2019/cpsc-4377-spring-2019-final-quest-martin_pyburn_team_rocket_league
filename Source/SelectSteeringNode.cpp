#include"SelectSteeringBehavior.h"
#include"Steering.h"
#include "GameObject.h"
#include<random>
SelectSteering::SelectSteering(GameObject* owner) :Node(owner)
{
	
	type = SELECTOR;
}

SelectSteering::~SelectSteering()
{

}


void SelectSteering::update()
{
	random_device rdev;
	static default_random_engine e(rdev());
	static uniform_int_distribution<int>d(0, 0 + 100);
	int chance = d(e);
	
	shared_ptr<Steering> steering = _owner->getComponent<Steering>();
	if (chance >= 50)
	{
		steering->changeState(steering->FLEE);
	}
	else
	{
		steering->changeState(steering->CHASE);
	}
}