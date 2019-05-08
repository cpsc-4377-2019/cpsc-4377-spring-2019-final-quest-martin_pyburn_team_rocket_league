#ifndef AUTOFIRE_H
#define AUTOFIRE_H

#include "Component.h"

class GameObject;
class ObjectParams;

class AutoFire : public Component {
public:
	AutoFire(std::shared_ptr<GameObject>);
	~AutoFire();

	bool initialize(ObjectParams*);

	void start();
	std::shared_ptr<GameObject> update();
	void finish();

private:
	float range{ 100.f };
	float precision{ 10.f };
};

#endif // !AUTOFIRE_H
