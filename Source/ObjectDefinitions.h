#ifndef OBJECTDEFINITIONS_H
#define OBJECTDEFINITIONS_H

#include "Definitions.h"

enum objectTypes { SHIP, PLANETOID, ENEMY, POWERUP, BULLET, MISSILE, COMPONENT, NA };
const map <string, int> objectTypesIndex{
	{ "Ship", objectTypes::SHIP },
	{ "Planetoid", objectTypes::PLANETOID },
	{ "Enemy", objectTypes::ENEMY },
	{ "PowerUp", objectTypes::POWERUP },
	{ "Bullet", objectTypes::BULLET },
	{ "Missile", objectTypes::MISSILE },
	{ "Component", objectTypes::COMPONENT }
};

enum objectComponents { BODY, SPRITE, USERINPUT, CHILDREN, CHASEBEHAVIOR, EVADEBEHAVIOR, STEERBEHAVIOR, EXPIRE, INTEGRITY, FRAGBEHAVIOR, PEASHOOTER, MISSILELAUNCHER, ACCELERATE, DRONE, AUTOFIRE, FORCEFIELD, END };
const map <int, string> objectComponentsIndex{
	{ objectComponents::BODY, "Body" },
	{ objectComponents::SPRITE, "Sprite" },
	{ objectComponents::USERINPUT, "UserInput" },
	{ objectComponents::CHILDREN, "Children" },
	{ objectComponents::CHASEBEHAVIOR, "ChaseBehavior" },
	{ objectComponents::EVADEBEHAVIOR, "EvadeBehavior" },
	{ objectComponents::STEERBEHAVIOR, "SteerBehavior" },
	{ objectComponents::EXPIRE, "Expire" },
	{ objectComponents::INTEGRITY, "Integrity" },
	{ objectComponents::FRAGBEHAVIOR, "FragBehavior" },
	{ objectComponents::PEASHOOTER, "PeaShooter" },
	{ objectComponents::MISSILELAUNCHER, "MissileLauncher" },
	{ objectComponents::ACCELERATE, "Accelerate" },
	{ objectComponents::DRONE, "Drone" },
	{ objectComponents::AUTOFIRE, "AutoFire" },
	{ objectComponents::FORCEFIELD, "ForceField" }
};
static vector<int> muted_components { CHILDREN, USERINPUT, FRAGBEHAVIOR, MISSILELAUNCHER, PEASHOOTER, PEASHOOTER, DRONE };

struct ObjectParams {
	float	x;
	float	y;
	float	angle;
	float	vx;
	float	vy;
	float	vangle;
	float	cx;
	float	cy;
	float	sx;
	float	sy;
	int		sw;
	int		sh;
	Uint32	color;
	bool	colorSet{ false };
	float	value;
	string	path;
	float	density{ 1.f };
	float	ldamp{ 0.f };
	float	adamp{ 0.f };
	int		radius{ 0 };
	int		layer;
	bool	shell{ 0 };
	bool	set{ 0 };
	vector<float> points;
};

class ObjectTemplate : public vector<shared_ptr<ObjectParams>> {
public:
	ObjectTemplate() {
		for (int a = 0; a < objectComponents::END; a++)
			this->push_back(make_shared<ObjectParams>());
	}
};

#endif