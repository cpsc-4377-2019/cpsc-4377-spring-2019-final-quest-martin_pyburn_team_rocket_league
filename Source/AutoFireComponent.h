#ifndef AUTOFIRE_H
#define AUTOFIRE_H

#include "Component.h"
#include "Definitions.h"

class GameObject;
class ObjectTemplate;

class AutoFire : public Component {
public:
	AutoFire(std::shared_ptr<GameObject>);
	~AutoFire();

	void initialize(std::shared_ptr<resource_map> resources, ObjectTemplate* temp);

	void start();
	std::shared_ptr<GameObject> update();
	void finish();

private:
	float range{ 100.f };
	float precision{ 10.f };
};

#endif // !AUTOFIRE_H
