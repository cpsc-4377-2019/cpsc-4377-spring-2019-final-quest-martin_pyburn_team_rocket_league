#include "MissileLauncher.h"
#include "GameObject.h"
#include "ObjectFactory.h"
#include "RidgidBodyComponent.h"
#include <chrono>

using namespace chrono;

MissileLauncher::MissileLauncher(std::shared_ptr<GameObject> owner) : Component( owner )
{
	missileTemplate = make_shared<ObjectTemplate>();
	ObjectParams* body = (*missileTemplate)[BODY].get();
	body->points = vector<float>{0.f,1.f,0.f,-1.f,8.f,-1.f,8.f,1.f};
	body->density = 50.f;
	body->set = true;
	ObjectParams* sprite = (*missileTemplate)[SPRITE].get();
	sprite->path = "ammo.png";
	sprite->sx = sprite->sy = 0.f;
	sprite->sw = 8.f;
	sprite->sh = 2.f;
	sprite->set = true;
	ObjectParams* expire = (*missileTemplate)[EXPIRE].get();
	expire->value = 2000;
	expire->set = true;
	ObjectParams* accelerate = (*missileTemplate)[ACCELERATE].get();
	accelerate->set = true;
	accelerate->x = 50.0f;
}

MissileLauncher::~MissileLauncher() { }

void MissileLauncher::start() { }

void MissileLauncher::finish() { }

void MissileLauncher::initialize(shared_ptr<resource_map> resources, ObjectTemplate* temp)
{
	this->factory = resources->factory.get();
}

std::shared_ptr<GameObject> MissileLauncher::update()
{
	// reload
	int tic = SDL_GetTicks();
	if(( tic - last_reload ) >= reload_interval && ammo < capacity ){
		ammo++;
		last_reload = tic;
	}
	return nullptr;
}

shared_ptr<GameObject> MissileLauncher::fire()
{
	if (getOwner()->pass_through != nullptr)return nullptr;
	// get current time
	int tic = SDL_GetTicks();
	if ((tic - last_fired) < fire_interval || ammo < 1 ) return nullptr;
	last_fired = tic;
	// attempt to create a missile
	shared_ptr<GameObject> missile = make_shared<GameObject>();
	factory->applyTemplate(missileTemplate, missile);
	if (missile != nullptr) {
		ammo--;
		// acquire the necessary components
		shared_ptr<RidgidBody> shipBody = getOwner()->getComponent<RidgidBody>();
		shared_ptr<RidgidBody> missileBody = missile->getComponent<RidgidBody>();
		if (shipBody == nullptr || missileBody == nullptr) return nullptr;

		// get ship data
		Vector2D ship_p = shipBody->getPosition();
		Vector2D ship_c = shipBody->getCentroid();
		Vector2D ship_v = shipBody->getVelocity();
		float ship_a = shipBody->getAngle();
		float ship_cos = cosf(ship_a * DEG_TO_RAD);
		float ship_sin = sinf(ship_a * DEG_TO_RAD);

		// position the missile
		missileBody->setTransform(
			ship_p + ship_c +
			Vector2D{
				ship_cos * 14,
				ship_sin * 14
			},
			ship_a
		);
		missileBody->setVelocity(
			Vector2D{
				// I'm not sure why we need to scale the velocity here
				ship_v.x * 0.01f,
				ship_v.y * 0.01f
			}
		);
		missileBody->setFilterIndex(shipBody->layer);
	}

	// apply Munition type
	missile->type = objectTypes::MISSILE;

	// apply damage
	missile->damage = missile_damage;

	// apply origin
	missile->origin = getOwner();

	// send it up
	return missile;
}

int MissileLauncher::getAmmoCount() { return ammo; }