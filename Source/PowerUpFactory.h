#ifndef POWERUPFACTORY_H
#define POWERUPFACTORY_H
#include "GameObject.h"
#include "ObjectDefinitions.h"
#include "ObjectFactory.h"

class PowerUpFactory {
public:
	PowerUpFactory(ObjectFactory* factory);
	~PowerUpFactory();
	std::shared_ptr<GameObject> getPower(GameObject* ship);

private:
	std::shared_ptr<GameObject> forceField(eFloat integrity = 100.f);
	std::shared_ptr<GameObject> drone();
	std::shared_ptr<GameObject> hp();
	std::shared_ptr<GameObject> mineral();

	enum powerName { P_SHIELD, P_DRONE, P_HP };

	std::map<powerName, weak_ptr<GameObject>> active;

	ObjectFactory* factory;
};

#endif // !POWERUPFACTORY_H
