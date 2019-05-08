#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include "Component.h"

class GameObject;
class ObjectFactory;
class ObjectTemplate;

class PeaShooter : public Component {
public:
	PeaShooter(std::shared_ptr<GameObject>);
	~PeaShooter();

	bool initialize(ObjectFactory* factory );

	void start();
	std::shared_ptr<GameObject> update();
	void finish();

	std::shared_ptr<GameObject> fire();


private:
	std::shared_ptr<ObjectTemplate> bulletTemplate;
	float bullet_speed = 5.f;
	int fire_interval = 150;	// milliseconds
	int last_fired = 0;
	ObjectFactory* factory;

	float bullet_damage = 10.f;	
};

#endif // !PEASHOOTER_H
