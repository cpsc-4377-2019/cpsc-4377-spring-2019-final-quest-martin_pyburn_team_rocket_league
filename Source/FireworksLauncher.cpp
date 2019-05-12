#include "FireworksLauncher.h"
#include "Firework.h"
#include "RidgidBodyComponent.h"
#include "GraphicsDevice.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include <random>

FWLauncher::FWLauncher(std::shared_ptr<GameObject> owner) : Component(owner)
{
}

FWLauncher::~FWLauncher()
{
}

bool FWLauncher::initialize(ObjectFactory* factory, GraphicsDevice* gDevice)
{
	this->gDevice = gDevice;
	this->factory = factory;
	delay = rand() % 10;
	return false;
}

void FWLauncher::start()
{
}

std::shared_ptr<GameObject> FWLauncher::update()
{
	if (getOwner()->pass_through == nullptr && !delay-- ) {
		delay = 25 + rand() % 125;
		// launch a firework
		shared_ptr<RidgidBody> obod = getOwner()->getComponent<RidgidBody>();
		if (obod == nullptr)return nullptr;
		Vector2D opos = obod->getPosition();
		shared_ptr<ObjectTemplate> ot = make_shared<ObjectTemplate>();
		ObjectParams* body = (*ot)[BODY].get();
		body->set = true;
		body->layer = -1;
		body->x = opos.x;
		body->y = opos.y;
		body->angle = obod->getAngle() * DEG_TO_RAD;
		body->radius = 5.f;
		body->cx = body->cy = 5.f;
		body->density = .1f;
		
		shared_ptr<GameObject> fw = make_shared<GameObject>();
		fw->type = objectTypes::COMPONENT;
		factory->applyTemplate(ot, fw);
		shared_ptr<Firework> fwc = make_shared<Firework>(fw);
		fwc->initialize(gDevice);
		fw->addComponent(fwc);
		fw->initialize();

		return fw;
	}
	return nullptr;
}

void FWLauncher::finish()
{
}

void FWLauncher::draw()
{
}
