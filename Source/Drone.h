#ifndef DRONE_H
#define DRONE_H

#include "Component.h"
#include "Definitions.h"

class GameObject;
class ObjectFactory;
class ObjectTemplate;
class Peashooter;

class Drone : public Component {
public:
	Drone(std::shared_ptr<GameObject>);
	~Drone();

	void initialize(std::shared_ptr<resource_map> resources, ObjectTemplate* temp);

	void start();
	std::shared_ptr<GameObject> update();
	void finish();



private:
	std::shared_ptr<ObjectTemplate> droneTemplate;
	ObjectFactory* factory;

	bool master{ false };
	bool engage{ false };

	std::shared_ptr<GameObject> aggro;
	std::shared_ptr<Drone> counterpart;

	float orbit = 80.f;
};

#endif // !DRONE_H
