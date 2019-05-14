#include "Steering.h"
#include "GameObject.h"
#include "DistanceSelectorNode.h"
#include "SoundDevice.h"
#include "RetreatNode.h"


Steering::Steering(std::shared_ptr<GameObject> owner):Component(owner)
{
	this->resources = resources;
	distSel = make_unique<DistanceSelector>(owner.get());
	distSel->setChaseRadius(radius);
	distSel->setFleeRadius(0.f);

	retrSel = make_unique<Retreat>(owner.get());
}

Steering::~Steering()
{

}

void Steering::initialize(shared_ptr<resource_map> resources, ObjectTemplate* temp)
{
	this->resources = resources;
	ObjectParams* steer = (*temp)[STEERBEHAVIOR].get();
	if (steer->radius > 0.f)radius = steer->radius;
	if (steer->value > 0.f)retreat = steer->value;
	// make sure we have a chase component
	chase = getOwner()->getComponent<Chase>();
	if (chase == nullptr) {
		shared_ptr<Chase> newChase = make_shared<Chase>(getOwner());
		newChase->initialize(resources, temp);
		chase = dynamic_pointer_cast<Chase>(getOwner()->addComponent(dynamic_pointer_cast<Component>(newChase)));
	}
	// make sure we have an evade component
	evade = getOwner()->getComponent<Evade>();
	if (evade == nullptr) {
		shared_ptr<Evade> newEvade = make_shared<Evade>(getOwner());
		newEvade->initialize(resources, temp);
		evade = dynamic_pointer_cast<Evade>(getOwner()->addComponent(dynamic_pointer_cast<Component>(newEvade)));
	}
	chase->distance = radius;
	evade->distance = 0.f;
}

void Steering::start()
{

}

void Steering::finish()
{

}

std::shared_ptr<GameObject> Steering::update()
{
	distSel->update();
	retrSel->update();
	return nullptr;
}

void Steering::changeState(States state)
{
	shared_ptr<GameObject> aggro = getOwner()->getAggro();
	if (aggro == nullptr)return;

	shared_ptr<RidgidBody> body = getOwner()->getComponent<RidgidBody>();
	shared_ptr<RidgidBody> tbod = aggro->getComponent<RidgidBody>();
	Vector2D tpos = tbod->getPosition();
	eFloat dist = body->getDistance(tpos);
	shared_ptr<Chase> chase = getOwner()->getComponent<Chase>();
	shared_ptr<Evade> evade = getOwner()->getComponent<Evade>();
	switch (state)
	{
	case States::CHASE:
		mode = States::CHASE;
		evade->distance = 0.f;
		chase->setActivation(true);
		evade->setActivation(false);
		break;

	case States::FLEE:
		resources->sounds->playSound(string("alien_01.ogg"));
		mode = States::FLEE;
		distSel->setFleeRadius(dist + retreat);
		evade->distance = 9999.f;
		chase->setActivation(false);
		evade->setActivation(true);
		break;

	case States::NA:
		mode = States::NA;
		chase->setActivation(false);
		evade->setActivation(false);
		break;

	}
}

Steering::States Steering::getMode()
{
	return mode;
}
