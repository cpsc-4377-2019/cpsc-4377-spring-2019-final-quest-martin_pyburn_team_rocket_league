#ifndef ACCELERATE_H
#define	ACCELERATE_H

#include "Component.h"

class GameObject;

class Accelerate : public Component {
public:
	Accelerate(std::shared_ptr<GameObject>);
	~Accelerate();

	void initialize(ObjectTemplate*);

	void start();
	void finish();

	std::shared_ptr<GameObject> update();

private:
	float acceleration;
};

#endif // !ACCELERATE_H
