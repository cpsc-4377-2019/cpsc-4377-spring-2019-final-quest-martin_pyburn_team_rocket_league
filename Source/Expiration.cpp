#include "GameObject.h"
#include "Expiration.h"
#include "ObjectDefinitions.h"
#include "RidgidBodyComponent.h"
#include "ObjectFactory.h"
#include "ParticleEmitter.h"
#include <chrono>

using namespace chrono;

Expire::Expire(std::shared_ptr<GameObject> owner) : Component( owner )
{
}

Expire::~Expire()
{
}

void Expire::start()
{
	the_end += SDL_GetTicks();
	counting = true;
}

void Expire::finish()
{
}

void Expire::initialize(ObjectFactory* factory, GraphicsDevice* gDevice, ObjectTemplate* temp )
{
	this->factory = factory;
	this->gDevice = gDevice;

	if( (*temp)[ EXPIRE ]->set )
		the_end = (*temp)[ EXPIRE ]->value;
}

std::shared_ptr<GameObject> Expire::update()
{
	shared_ptr<GameObject> owner = getOwner();
	if (!dead && (owner->distress || counting && SDL_GetTicks() >= the_end)) {
		dead = true;
		cout << "missile dying" << endl;
		if(owner->type == objectTypes::MISSILE)
			return spawnEmitter();
	}
	if (dead) owner->live = false;
	return nullptr;
}

std::shared_ptr<GameObject> Expire::spawnEmitter()
{
	shared_ptr<RidgidBody> obod = getOwner()->getComponent<RidgidBody>();
	if (obod == nullptr)return nullptr;
	Vector2D opos = obod->getPosition();
	shared_ptr<ObjectTemplate> ot = make_shared<ObjectTemplate>();
	ObjectParams* body = (*ot)[BODY].get();
	body->set = true;
	body->layer = -1;
	body->x = opos.x;
	body->y = opos.y;

	shared_ptr<GameObject> em = make_shared<GameObject>();
	em->type = objectTypes::COMPONENT;
	factory->applyTemplate(ot, em);
	shared_ptr<ParticleParams> params = make_shared<ParticleParams>();
	eFloat area = obod->getArea();
	params->lifespan = 8;
	params->parttime = 64;
	params->ppf = 30;
	params->texture = "./Assets/Images/puff.png";
	params->speed = 20;
	params->x = opos.x;
	params->y = opos.y;
	params->rx = params->ry = 0;
	params->rw = params->rh = 5;
	params->cx = params->cy = 2.5f;
	params->color = RGBA{ 196, 255, 64, 255 };
	params->endcol = RGBA{ 128, 64 ,32 ,255 };
	shared_ptr<Emitter> emc = make_shared<Emitter>(em);
	emc->initialize(gDevice, params);
	em->addComponent(emc);

	return em;
}

