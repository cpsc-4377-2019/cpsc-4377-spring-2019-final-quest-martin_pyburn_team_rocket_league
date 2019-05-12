#include "IntegrityComponent.h"
#include "ObjectDefinitions.h"
#include "ObjectListComponent.h"
#include "ObjectFactory.h"
#include "RidgidBodyComponent.h"
#include "ParticleEmitter.h"
#include "GraphicsDevice.h"
#include "GameObject.h"
#include "Component.h"

Integrity::Integrity(std::shared_ptr<GameObject> owner) : Component(owner)
{
}

Integrity::~Integrity()
{
}

bool Integrity::initialize(ObjectFactory* factory, GraphicsDevice* gDevice, ObjectParams* params)
{
	integrity = params->value;
	this->factory = factory;
	this->gDevice = gDevice;
	return false;
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
	factory->applyTemplate(ot, em);
	shared_ptr<ParticleParams> params = make_shared<ParticleParams>();
	eFloat area = obod->getArea();
	params->lifespan = fmax(area / 50, 5);
	params->parttime = fmax(area, 30);
	params->ppf = fmax(area / 30, 10);
	params->texture = "./Assets/Images/puff.png";
	params->speed = 10;
	params->x = opos.x;
	params->y = opos.y;
	params->rx = params->ry = 0;
	params->rw = params->rh = 5;
	params->cx = params->cy = 2.5f;
	params->color = RGBA{ 196, 196, 196, 255 };
	params->endcol = RGBA{ 64, 64 ,64 ,0 };
	shared_ptr<Emitter> emc = make_shared<Emitter>(em);
	emc->initialize(gDevice, params);
	em->addComponent(emc);

	return em;
}
