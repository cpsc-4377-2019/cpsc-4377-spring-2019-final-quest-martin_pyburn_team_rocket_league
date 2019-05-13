#ifndef ACCELERATE_H
#define	ACCELERATE_H

#include "Component.h"

class GameObject;
struct resource_map;

class Accelerate : public Component {
public:
	Accelerate(std::shared_ptr<GameObject>);
	~Accelerate();

	void initialize(std::shared_ptr<resource_map>, ObjectTemplate*);

	void start();
	void finish();

	std::shared_ptr<GameObject> update();

private:
	float acceleration;
};

#endif // !ACCELERATE_H
