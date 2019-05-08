#include "PowerUpFactory.h"
#include "ForceFieldComponent.h"
#include "Drone.h"
#include <random>

PowerUpFactory::PowerUpFactory(ObjectFactory * factory)
{
	this->factory = factory;
}

PowerUpFactory::~PowerUpFactory(){}

std::shared_ptr<GameObject> PowerUpFactory::getPower(GameObject * ship)
{
	if (rand() % 2) {
		if (ship->getComponent<ForceField>() == nullptr && active[P_SHIELD].lock() == nullptr) {
			shared_ptr<GameObject> re = forceField();
			active[P_SHIELD] = re;
			return re;
		}
		else if (ship->getComponent<Drone>() == nullptr && active[P_DRONE].lock() == nullptr) {
			shared_ptr<GameObject> re = drone();
			active[P_DRONE] = re;
			return re;
		}
	}
	if (rand() % 5 == 0) return hp();
	if (rand() % 5 == 0) return mineral();
}

std::shared_ptr<ObjectTemplate> baseTemplate() {
	std::shared_ptr<ObjectTemplate> temp = make_shared<ObjectTemplate>();
	ObjectParams* body = (*temp)[BODY].get();
	body->ldamp = 5.f;
	body->adamp = 500.f;
	body->set = true;
	body->cx = body->cy = 15.f;
	ObjectParams* sprite = (*temp)[SPRITE].get();
	sprite->set = true;
	sprite->sw = sprite->sh = 30.f;
	return temp;
}

std::shared_ptr<GameObject> PowerUpFactory::forceField(eFloat integrity) {
	std::shared_ptr<GameObject> obj = make_shared<GameObject>();
	obj->type = objectTypes::POWERUP;

	std::shared_ptr<ObjectTemplate> temp = baseTemplate();

	ObjectParams* field = (*temp)[FORCEFIELD].get();
	field->set = true;
	field->value = integrity;

	(*temp)[SPRITE]->path = "P_SHIELD.png";

	factory->applyTemplate(temp, obj);
	return obj;
}

std::shared_ptr<GameObject> PowerUpFactory::drone() {

	std::shared_ptr<GameObject> obj = make_shared<GameObject>();
	obj->type = objectTypes::POWERUP;

	std::shared_ptr<ObjectTemplate> temp = baseTemplate();

	(*temp)[DRONE]->set = true;

	(*temp)[SPRITE]->path = "P_DRONE.png";

	factory->applyTemplate(temp, obj);
	return obj;
}

std::shared_ptr<GameObject> PowerUpFactory::hp()
{
	std::shared_ptr<GameObject> obj = make_shared<GameObject>();
	obj->type = objectTypes::POWERUP;
	obj->damage = -100.f;

	std::shared_ptr<ObjectTemplate> temp = baseTemplate();

	(*temp)[SPRITE]->path = "P_HP.png";

	factory->applyTemplate(temp, obj);
	return obj;
}

std::shared_ptr<GameObject> PowerUpFactory::mineral()
{
	std::shared_ptr<GameObject> obj = make_shared<GameObject>();
	obj->type = objectTypes::POWERUP;
	obj->damage = -10.f;

	// ADD TO SCORE

	std::shared_ptr<ObjectTemplate> temp = baseTemplate();

	(*temp)[SPRITE]->path = "mineral.png";

	factory->applyTemplate(temp, obj);
	return obj;
}
