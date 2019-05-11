#include "ContactListener.h"
#include "Component.h"
#include "GameObject.h"
#include "RidgidBodyComponent.h"
#include "IntegrityComponent.h"
#include "PeaShooter.h"
#include "MissileLauncher.h"
#include "PowerUpComponent.h"

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	//Grab the two Physics Bodies involved in the Contact
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();

	// Don't do anything special with boundaries
	if (bodyA->GetType() == b2BodyType::b2_staticBody || bodyB->GetType() == b2BodyType::b2_staticBody)
		return;

	//Cast them to object pointers
	std::shared_ptr<GameObject> objectA = static_cast<Component*>(bodyA->GetUserData())->getOwner();
	std::shared_ptr<GameObject> objectB = static_cast<Component*>(bodyB->GetUserData())->getOwner();

	if (objectA == nullptr || objectB == nullptr)return;
	bool proceed{ true };

	// weed out ghost hulls
	if (objectA->type > objectTypes::NA) {
		proceed = false;
		bodyA->SetActive(false);
	}
	if (objectB->type > objectTypes::NA) {
		proceed = false;
		bodyB->SetActive(false);
	}
	//cout << objectA->type << " " << objectB->type << endl;

	if(proceed) switch (actions[objectA->type][objectB->type]) {
	case actionType::B_O: // bullet vs object
		BulletHit(objectA, objectB);
		break;
	case actionType::O_B: // object vs bullet
		BulletHit(objectB, objectA);
		break;
	case actionType::M_O: // missile vs object
		MissileHit(objectA, objectB);
		break;
	case actionType::O_M: // object vs missile
		MissileHit(objectB, objectA);
		break;
	case actionType::S_O: // ship vs object
		ShipCrash(objectA, objectB);
		break;
	case actionType::O_S: // object vs ship
		ShipCrash(objectB, objectA);
		break;
	case actionType::S_P: // ship vs powerup
		GetPowerUp(objectA, objectB);
		break;
	case actionType::P_S: // powerup vs ship
		GetPowerUp(objectB, objectA);
		break;
	case actionType::NA:break;
	default:
		break;
	}
}

void ContactListener::BulletHit(std::shared_ptr<GameObject> bullet, std::shared_ptr<GameObject> target)
{
	shared_ptr<Integrity> targeti = target->getComponent<Integrity>();
	if(targeti != nullptr) targeti->damage(bullet->damage);
	if (target->live && bullet->origin != nullptr && bullet->origin->type == objectTypes::SHIP) {
		bullet->origin->aggro = target;
	}
	if (!target->live) {
		bullet->origin->score += target->points;
	}
}

void ContactListener::MissileHit(std::shared_ptr<GameObject> missile, std::shared_ptr<GameObject> target)
{
	shared_ptr<Integrity> targeti = target->getComponent<Integrity>();
	if (targeti != nullptr) targeti->damage(missile->damage);

	if (!target->live) {
		missile->origin->score += target->points;
	}
	// generate particles...

	// kill the missile
	missile->live = false;
}

void ContactListener::ShipCrash(std::shared_ptr<GameObject> ship, std::shared_ptr<GameObject> notship)
{
	// Allow shields to take damage
	shared_ptr<Integrity> shipi = ship->getComponent<Integrity>();
	shared_ptr<Integrity> noti = notship->getComponent<Integrity>();
	shared_ptr<RidgidBody> sbody = ship->getComponent<RidgidBody>();
	shared_ptr<RidgidBody> nbody = notship->getComponent<RidgidBody>();
	if (shipi != nullptr && noti != nullptr && sbody != nullptr && nbody != nullptr) {
		// calculate speed of impact to scale damage
		Vector2D spos = sbody->getPosition();
		Vector2D npos = nbody->getPosition();
		Vector2D svel = sbody->getVelocity();
		Vector2D nvel = nbody->getVelocity();
		float angle = atan2f(spos.y - npos.y, spos.x - npos.x);
		float mag = sqrtf(powf((svel.x - nvel.x) * cosf(angle), 2.f) + powf((svel.y - nvel.y) * sinf(angle), 2.f));
		if (mag > 30.f) {
			float sdamage = notship->damage + nbody->getArea() / 500.f * mag;
			shipi->damage(sdamage);
			float ndamage = ship->damage + sbody->getArea() / 500.f * mag;
			if (noti != nullptr) noti->damage(ndamage);
			if (!notship->live) {
				ship->score += notship->points;
			}
		}
	}
}

void ContactListener::GetPowerUp(std::shared_ptr<GameObject> ship, std::shared_ptr<GameObject> powerup)
{	// If a powerup hits a component, pass it up to the owner of the component
	if(ship->type == objectTypes::COMPONENT)ship = ship->origin;
	if (ship == nullptr) return;

	// add score
	ship->score += powerup->points;

	powerup->live = false;
	shared_ptr<Integrity> shipi = ship->getComponent<Integrity>();
	if (shipi != nullptr)
		shipi->damage(-abs(powerup->damage));
	shared_ptr<PowerUp> power = powerup->getComponent<PowerUp>();
	if (power != nullptr)
		power->empower(ship);
}
