#include "PeaShooter.h"
#include "GameObject.h"
#include "ObjectFactory.h"
#include "RidgidBodyComponent.h"
#include "Drone.h"

PeaShooter::PeaShooter(std::shared_ptr<GameObject> owner) : Component( owner )
{
	bulletTemplate = make_shared<ObjectTemplate>();
	ObjectParams* body = (*bulletTemplate)[BODY].get();
	body->density = 0.1f;
	body->set = true;
	ObjectParams* sprite = (*bulletTemplate)[SPRITE].get();
	sprite->path = "ammo.png";
	sprite->sx = sprite->sy = 0.f;
	sprite->sw = sprite->sh = 2.f;
	sprite->set = true;
	ObjectParams* expire = (*bulletTemplate)[EXPIRE].get();
	expire->value = 400;
	expire->set = true;
}

PeaShooter::~PeaShooter()
{
}

bool PeaShooter::initialize(ObjectFactory* factory)
{
	this->factory = factory;
	return factory != nullptr;
}

void PeaShooter::start(){
}

std::shared_ptr<GameObject> PeaShooter::update(){

	return nullptr;
}

void PeaShooter::finish() {}

std::shared_ptr<GameObject> PeaShooter::fire()
{
	if (getOwner()->pass_through != nullptr)return nullptr;
	// get current time
	int tic = SDL_GetTicks();
	if ((tic - last_fired) < fire_interval) return nullptr;
	last_fired = tic;
	// attempt to create a bullet
	shared_ptr<GameObject> bullet = make_shared<GameObject>();
	factory->applyTemplate(bulletTemplate, bullet);
	if (bullet != nullptr) {
		// acquire the necessary components
		shared_ptr<RidgidBody> shipBody = getOwner()->getComponent<RidgidBody>();
		shared_ptr<RidgidBody> bulletBody = bullet->getComponent<RidgidBody>();
		if (shipBody == nullptr || bulletBody == nullptr) return nullptr;
	
		// get ship data
		Vector2D ship_p = shipBody->getPosition();
		Vector2D ship_c = shipBody->getCentroid();
		Vector2D ship_v = shipBody->getVelocity();
		float ship_a	= shipBody->getAngle() * DEG_TO_RAD;
		float ship_cos	= cosf(ship_a);
		float ship_sin	= sinf(ship_a);

		// position the bullet
		bulletBody->setTransform(
			ship_p + ship_c +
			Vector2D{
				ship_v.x * 0.01f + ship_cos * 14,
				ship_v.y * 0.01f + ship_sin * 14
			},
			ship_a
		);
		bulletBody->setVelocity(
			Vector2D{ ship_cos * bullet_speed, ship_sin * bullet_speed }
		);
		bulletBody->setFilterIndex(shipBody->layer);
	}

	// apply Munition type
	bullet->type = objectTypes::BULLET;

	// apply origin
	bullet->origin = getOwner();

	// apply damage
	bullet->damage = bullet_damage;

	return bullet;
}
