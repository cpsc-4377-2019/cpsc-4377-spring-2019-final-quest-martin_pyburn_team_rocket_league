#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include "Box2D.h"
#include <vector>

class  GameObject;
class ContactListener : public b2ContactListener
{
public:

	void BeginContact(b2Contact* contact) {}

	void EndContact(b2Contact* contact) {}

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {}

private:
	void BulletHit(std::shared_ptr<GameObject> bullet, std::shared_ptr<GameObject> target);
	void MissileHit(std::shared_ptr<GameObject> missile, std::shared_ptr<GameObject> target);
	void ShipCrash(std::shared_ptr<GameObject> ship, std::shared_ptr<GameObject> notship);
	void GetPowerUp(std::shared_ptr<GameObject> ship, std::shared_ptr<GameObject> powerup);

	enum actionType { B_O, O_B, M_O, O_M, S_O, O_S, S_P, P_S, NA };
	std::vector<std::vector<actionType>> actions{
		//				SHIP	PLANET	ENEMY	POWERUP BULLET	MISSILE	COMP	NA
		/* SHIP vs: */{	NA,		S_O,	S_O,	S_P,	O_B,	NA,		S_O,	NA},
		/* PLAN vs: */{	O_S,	NA,		NA,		NA,		O_B,	O_M,	NA,		NA},
		/* ENEM vs: */{	O_S,	NA,		NA,		S_P,	O_B,	O_M,	NA,		NA},
		/* POWE vs: */{	P_S,	NA,		P_S,	NA,		NA,		NA,		P_S,	NA},
		/* BULL vs: */{	B_O,	B_O,	B_O,	NA,		NA,		NA,		B_O,	NA},
		/* MISS vs: */{	NA,		M_O,	M_O,	NA,		NA,		NA,		NA,		NA},
		/* COMP vs: */{	O_S,	NA,		NA,		S_P,	O_B,	NA,		NA,		NA},
		/* NA   vs: */{	NA,		NA,		NA,		NA,		NA,		NA,		NA,		NA}
	};
};

#endif