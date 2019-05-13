#include "PowerUpComponent.h"
#include "ObjectDefinitions.h"
#include "ObjectFactory.h"
#include "Definitions.h"
#include "GameObject.h"
#include "RidgidBodyComponent.h"

PowerUp::PowerUp(std::shared_ptr<GameObject> owner) : Component(owner)
{
}

PowerUp::~PowerUp()
{
}

void PowerUp::initialize(ObjectFactory* factory, shared_ptr<ObjectTemplate> temp)
{
	this->factory = factory;
	components = temp;
	(*components)[BODY]->set = false;
	(*components)[SPRITE]->set = false;
}

void PowerUp::start()
{
}

std::shared_ptr<GameObject> PowerUp::update()
{
	std::shared_ptr<RidgidBody> body = getOwner()->getComponent<RidgidBody>();
	if (body != nullptr)body->setAngle(0.f);
	return nullptr;
}

void PowerUp::finish()
{
}

bool PowerUp::empower(std::shared_ptr<GameObject> target)
{
	// may need to remove components that are already assigned
	factory->applyTemplate(components, target);
	return true;
}
