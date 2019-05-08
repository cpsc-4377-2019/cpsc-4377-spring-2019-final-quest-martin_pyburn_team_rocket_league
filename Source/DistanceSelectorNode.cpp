#include"DistanceSelectorNode.h"
#include "SelectSteeringBehavior.h"
#include "GameObject.h"
#include "Steering.h"

DistanceSelector::DistanceSelector(GameObject* owner) : Node(owner)
{
	type = SELECTOR;
	
}

DistanceSelector::~DistanceSelector()
{

}

void DistanceSelector::update()
{
	shared_ptr<GameObject> aggro = _owner->getAggro();
	if (aggro == nullptr)return;
	shared_ptr<Steering> steering = _owner->getComponent<Steering>();

	shared_ptr<RidgidBody> body = _owner->getComponent<RidgidBody>();
	shared_ptr<RidgidBody> tbody = aggro->getComponent<RidgidBody>();

	Vector2D aggroPos = tbody->getPosition();

	eFloat dist = body->getDistance(aggroPos);

	if (steering->getMode() != Steering::CHASE) {
		if (dist >= flee)steering->changeState(Steering::CHASE);
	}
}

void DistanceSelector::setChaseRadius(eFloat radius)
{
	chase = radius;
}

void DistanceSelector::setFleeRadius(eFloat radius)
{
	flee = radius;
}
