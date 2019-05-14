#include "AutoFireComponent.h"
#include "ObjectDefinitions.h"
#include "RidgidBodyComponent.h"
#include "GameObject.h"
#include "PeaShooter.h"

AutoFire::AutoFire(std::shared_ptr<GameObject> owner) : Component( owner)
{
}

AutoFire::~AutoFire()
{
}

void AutoFire::initialize(std::shared_ptr<resource_map> resources, ObjectTemplate* temp)
{
	ObjectParams* params = (*temp)[AUTOFIRE].get();
	range = params->radius;
	precision = params->value;
}

void AutoFire::start()
{
}

std::shared_ptr<GameObject> AutoFire::update()
{
	shared_ptr<GameObject> owner = getOwner();
	shared_ptr<GameObject> aggro = owner->getAggro();
	if (aggro != nullptr) {

		if (!aggro->live) return aggro = nullptr;

		shared_ptr<RidgidBody> aggroBody = aggro->getComponent<RidgidBody>();
		shared_ptr<RidgidBody> body = getOwner()->getComponent<RidgidBody>();
		Vector2D aggroPos = aggroBody->getPosition();
		Vector2D bodyPos = body->getPosition();
		float distX = aggroPos.x - bodyPos.x;
		float distY = aggroPos.y - bodyPos.y;

		// check if we're pointed the right way
		float angle = body->getRelativeAngle(aggroPos);
		if (fabsf(angle) > precision) return nullptr;

		// determine range to aggro
		float dist = sqrtf(distX * distX + distY * distY);

		if (dist <= range) { // in range, ATTACK!!!!!
			shared_ptr<PeaShooter> peashooter = getOwner()->getComponent<PeaShooter>();
			if (peashooter != nullptr) {
				return peashooter->fire();
			}
		}
	}
	return nullptr;
}

void AutoFire::finish()
{
}
