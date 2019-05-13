#ifndef POWERUPCOMPONENT_H
#define POWERUPCOMPONENT_H

#include "Definitions.h"
#include "Component.h"

class ObjectTemplate;
class ObjectFactory;

class PowerUp : public Component {

public:
	PowerUp(std::shared_ptr<GameObject>);
	~PowerUp();
	void initialize(std::shared_ptr<resource_map>, ObjectTemplate*) {};
	void initialize(ObjectFactory*, shared_ptr<ObjectTemplate> temp);

	void start();
	std::shared_ptr<GameObject> update();
	void finish();

	bool empower(std::shared_ptr<GameObject>);

private:
	ObjectFactory* factory;

	std::shared_ptr<ObjectTemplate> components;
};

#endif // POWERUPCOMPONENT_H
