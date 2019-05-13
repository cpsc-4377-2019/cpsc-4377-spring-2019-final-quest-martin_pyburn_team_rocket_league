#include "GameObject.h"
#include "Expiration.h"
#include "ObjectDefinitions.h"
#include "RidgidBodyComponent.h"
#include "SoundDevice.h"
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

void Expire::initialize(shared_ptr<resource_map> resources, ObjectTemplate* temp)
{
	this->resources = resources;

	if( (*temp)[ EXPIRE ]->set )
		the_end = (*temp)[ EXPIRE ]->value;
}

std::shared_ptr<GameObject> Expire::update()
{
	shared_ptr<GameObject> owner = getOwner();
	if (!dead && (owner->distress || counting && SDL_GetTicks() >= the_end)) {
		dead = true;
		if(owner->type == objectTypes::MISSILE)
			return spawnEmitter();
	}
	if (dead) owner->live = false;
	return nullptr;
}

std::shared_ptr<GameObject> Expire::spawnEmitter()
{
	resources->sounds->playSound(string("explosion_01.ogg"), 0, -1);

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
	resources->factory->applyTemplate(ot, em);
	shared_ptr<ParticleParams> params = make_shared<ParticleParams>();
	eFloat area = obod->getArea();
	params->lifespan = 15;
	params->parttime = 64;
	params->ppf = 30;
	params->texture = resources->imgPath + "puff.png";
	params->speed = 5;
	params->x = opos.x;
	params->y = opos.y;
	params->rx = params->ry = 0;
	params->rw = params->rh = 8;
	params->cx = params->cy = 2.5f;
	params->color = RGBA{ 255, 196, 64, 128 };
	params->endcol = RGBA{ 128, 0 ,0 ,255 };
	shared_ptr<Emitter> emc = make_shared<Emitter>(em);
	emc->initialize(resources->graphics.get(), params);
	em->addComponent(emc);

	return em;
}

