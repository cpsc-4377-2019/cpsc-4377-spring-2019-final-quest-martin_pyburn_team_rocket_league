#ifndef SELECTSTEERINGBEHAVIOR_H
#define SELECTSTEERINGBEHAVIOR_H
#include"Node.h"
#include"Steering.h"
class SelectSteering:public Node
{
public:
	SelectSteering(GameObject*);
	~SelectSteering();

	void update();
};
#endif // !SELECTSTEERINGBEHAVIOR_H

