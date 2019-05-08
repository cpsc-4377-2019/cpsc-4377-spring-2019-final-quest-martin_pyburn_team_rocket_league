#include "RetreatNode.h"
#include "Steering.h"
#include "IntegrityComponent.h"
#include "GameObject.h"

Retreat::Retreat(GameObject * owner) : Node(owner)
{
	type = SELECTOR;

}

Retreat::~Retreat()
{
}

void Retreat::update()
{
	shared_ptr<Steering> steering = _owner->getComponent<Steering>();
	
	if (steering == nullptr) return;

	if (_owner->distress) {
		steering->changeState(Steering::FLEE);
		frames = 200;
	}
	else frames--;
	if (frames == 0)steering->changeState(Steering::CHASE);

	_owner->distress = false;
}
