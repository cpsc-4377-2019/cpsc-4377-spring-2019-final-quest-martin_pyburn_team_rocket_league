#include "Accelerate.h"
#include "GameObject.h"
#include "RidgidBodyComponent.h"

Accelerate::Accelerate(std::shared_ptr<GameObject> owner) : Component( owner )
{
}

Accelerate::~Accelerate()
{
}

void Accelerate::initialize(ObjectTemplate* temp )
{
	acceleration = (*temp)[ACCELERATE]->x;
}

void Accelerate::start()
{
}

void Accelerate::finish()
{
}

std::shared_ptr<GameObject> Accelerate::update()
{
	std::shared_ptr<RidgidBody> body = getOwner()->getComponent<RidgidBody>();
	if (body == nullptr) return nullptr;
	eFloat angle = body->getAngle() * DEG_TO_RAD;
	body->applyImpulse(Vector2D{
			cosf(angle) * acceleration,
			sinf(angle) * acceleration
	});


	return nullptr;
}
