#ifndef STEERING_H
#define STEERING_H

#include"Component.h"
#include "ChaseBehavior.h"
#include "Evade.h"

class DistanceSelector;
class Retreat;

class Steering :public Component
{
public:
	enum States{CHASE, FLEE, NA};
	Steering(std::shared_ptr<GameObject>);
	~Steering();

	void initialize(shared_ptr<resource_map> resources, ObjectTemplate* temp);

	void start();
	void finish();

	std::shared_ptr<GameObject> update();
	void changeState(States state);
	States getMode();

private:
	float radius{ 100.f };
	float retreat{ 50.f };

	shared_ptr<resource_map> resources;

	States state = NA;
	std::unique_ptr<DistanceSelector> distSel;
	std::unique_ptr<Retreat> retrSel;

	States mode{ NA };

	std::shared_ptr<Chase> chase;
	std::shared_ptr<Evade> evade;

};
#endif // !STEERING_H

