#include "Evade.h"
#include "Engine.h"
#include "GameObject.h"

Evade::Evade(std::shared_ptr<GameObject> owner) : Component(owner) {}

Evade::~Evade()
{

}

void Evade::initialize(ObjectTemplate* temp) 
{
	ObjectParams* evade = (*temp)[EVADEBEHAVIOR].get();
	if (evade->radius > 0.f)distance = evade->radius;
}

void Evade::start()
{

}

void Evade::finish()
{

}

std::shared_ptr<GameObject> Evade::update()
{
	// check if action is needed
	shared_ptr<GameObject> aggro = getOwner()->aggro.lock();
	if (aggro == nullptr || !aggro->live || activated == false) return nullptr;
	
	shared_ptr<RidgidBody> body = getOwner()->getComponent<RidgidBody>();
	shared_ptr<RidgidBody> tbody = aggro->getComponent<RidgidBody>();

	eFloat imp = -speed * body->density;

	Vector2D aggrop = tbody->getPosition();
	Vector2D chaserp = body->getPosition();

	// if we're far enough away, we can stop evading
	eFloat dist = body->getDistance(aggrop);
	if (dist > distance) {
		return nullptr;
	}

	eFloat angle = -body->getAngle() * DEG_TO_RAD;;

	Vector2D direction{ imp * cosf(angle), imp * sinf(angle) };
	body->applyImpulse(direction);

	Vector2D current_vel = body->getVelocity();
	float dir = atan2f(current_vel.y, current_vel.x);
	Vector2D current_comp{ cosf(dir), sinf(dir) };

	float mag = sqrtf(powf(current_vel.x, 2) + powf(current_vel.y, 2));
	float citation = speedLimit - mag;
	if (citation < 0)
		body->applyImpulse(Vector2D{ current_comp.x * citation , current_comp.y * citation });

	eFloat relAngle = body->getRelativeAngle(aggrop);
	if (relAngle != 0.f) body->setRotation(relAngle / 2.5f);

	return nullptr;
}
