#ifndef DRONE_H
#define DRONE_H

#include "Component.h"

class GameObject;
class ObjectFactory;
class ObjectTemplate;
class Peashooter;

class Drone : public Component {
public:
	Drone(std::shared_ptr<GameObject>);
	~Drone();

	bool initialize(ObjectFactory* factory, ObjectTemplate* temp);

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
