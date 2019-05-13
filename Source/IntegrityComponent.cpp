#include "IntegrityComponent.h"
#include "ObjectDefinitions.h"
#include "ObjectListComponent.h"
#include "ObjectFactory.h"
#include "RidgidBodyComponent.h"
#include "ParticleEmitter.h"
#include "GraphicsDevice.h"
#include "SoundDevice.h"
#include "GameObject.h"
#include "Component.h"

Integrity::Integrity(std::shared_ptr<GameObject> owner) : Component(owner)
{
}

Integrity::~Integrity()
{
}

void Integrity::initialize(shared_ptr<resource_map> resources, ObjectTemplate* temp)
{
	integrity = (*temp)[INTEGRITY]->value;
	this->resources = resources;
}

void Integrity::start()
{
}

std::shared_ptr<GameObject> Integrity::update()
{
	shared_ptr<GameObject> owner = getOwner();
	if (!dead && integrity <= 0 && owner->pass_through == nullptr) {
		dead = true;
		return spawnEmitter();
	}
	if (dead)owner->live = false;
	return nullptr;
}

void Integrity::finish()
{
}

void Integrity::damage(float dmg) {
	getOwner()->distress = true;
	if (integrity > 0) integrity -= dmg;
}

shared_ptr<GameObject> Integrity::spawnEmitter()
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
	resources->factory->applyTemplate(ot, em);
	shared_ptr<ParticleParams> params = make_shared<ParticleParams>();
	eFloat area = obod->getArea();
	params->lifespan = fmax(area / 30, 10);
	params->parttime = fmax(area, 60);
	params->ppf = fmax(area / 30, 10);
	params->texture = resources->imgPath + "puff.png";
	params->speed = 4;
	params->x = opos.x;
	params->y = opos.y;
	params->rx = params->ry = 0;
	params->rw = params->rh = 15;
	params->cx = params->cy = 2.5f;
	params->color = RGBA{ 196, 196, 196, 128 };
	params->endcol = RGBA{ 64, 64 ,64 ,0 };
	shared_ptr<Emitter> emc = make_shared<Emitter>(em);
	emc->initialize(resources->graphics.get(), params);
	em->addComponent(emc);

	return em;
}
