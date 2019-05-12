#include "Firework.h"
#include "GameObject.h"
#include "ParticleEmitter.h"
#include "GraphicsDevice.h"
#include "RidgidBodyComponent.h"

Firework::Firework(std::shared_ptr<GameObject> owner) : Component(owner)
{
	this->gDevice = gDevice;
}

Firework::~Firework()
{
}

bool Firework::initialize(GraphicsDevice * gDevice)
{
	this->gDevice = gDevice;
	shared_ptr<RidgidBody> body = getOwner()->getComponent<RidgidBody>();
	Vector2D bodpos = body->getPosition();
	shared_ptr<GameObject> fireworks = getOwner();
	shared_ptr<Emitter> emitter = make_shared<Emitter>(fireworks);
	shared_ptr<ParticleParams> params = make_shared<ParticleParams>();
	params->x = bodpos.x;
	params->y = bodpos.y;
	params->texture = "./Assets/Images/puff.png";
	params->cx = params->cy = 2.5f;
	params->rx = params->ry = 0;
	params->rw = params->rh = 5;
	params->lifespan = -1;
	params->parttime = 30;
	params->ppf = 1;
	params->range = .3f;
	params->speed = 1.f;
	params->gravity = { 0.f, 0.25f };
	params->color.setRGB(0xffff88);
	params->endcol.setRGB(0xff3030);
	emitter->initialize(gDevice, params);
	emitter->setActivation(true);
	fireworks->addComponent(emitter);
	fireworks->initialize();

	burst = 45 + rand() % 45;
	return false;
}

void Firework::start()
{
}

void Firework::finish()
{
}

shared_ptr<GameObject> Firework::update()
{
	shared_ptr<Emitter> emitter = getOwner()->getComponent<Emitter>();
	shared_ptr<RidgidBody> body = getOwner()->getComponent<RidgidBody>();
	if (emitter == nullptr || body == nullptr) return nullptr;
	if (age++ == burst) {
		// ROCKETS EXPLODE!!!
		emitter->setLifespan(30 + rand() % 20);

		body->setActive(false);
		// Convert to spark shower
		emitter->setParttime(50);
		emitter->setRange(360.f);
		emitter->setPPF(20);
		emitter->setSpeed(5 + rand() % 5);
		emitter->setColors( randCol(), randCol() );
	}
	else {
		// update rocket
		eFloat angle = body->getAngle();
		Vector2D vel = body->getVelocity();
		eFloat dir = atan2f(vel.y, vel.x);
		body->applyImpulse({cosf(angle * DEG_TO_RAD),sinf(angle * DEG_TO_RAD)});
		emitter->setPosition(body->getPosition());
		emitter->setAngle(dir * RAD_TO_DEG + 180.f);
	}
	return nullptr;
}

void Firework::draw()
{
	shared_ptr<Emitter> emitter = getOwner()->getComponent<Emitter>();
	if (emitter == nullptr)return;
	emitter->draw();
}

RGBA Firework::randCol()
{
	return { randVal(), randVal(), randVal(), 255 };
}

eInt Firework::randVal()
{
	return 64 + (rand() % 6) * 32;
}
