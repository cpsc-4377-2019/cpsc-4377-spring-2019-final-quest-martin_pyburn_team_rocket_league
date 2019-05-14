#include "UserInputComponent.h"
#include "InputHandler.h"
#include "GraphicsDevice.h"
#include "GameObject.h"
#include "RidgidBodyComponent.h"
#include "SoundDevice.h"
#include "PeaShooter.h"
#include "MissileLauncher.h"
#include "ParticleEmitter.h"

UserInput::UserInput(std::shared_ptr<GameObject> owner) : Component( owner ) { }

UserInput::~UserInput() { }

void UserInput::initialize(shared_ptr<resource_map> resources, ObjectTemplate* temp)
{
	this->resources = resources;
	shared_ptr<ParticleParams> params = make_shared<ParticleParams>();
	params->texture = resources->imgPath + "puff.png";
	params->ppf = 1;
	params->lifespan = -1;
	params->parttime = 25;
	params->rx = 0;
	params->ry = 0;
	params->rw = 8;
	params->rh = 8;
	params->cx = 4.f;
	params->cy = 4.f;
	params->angle = 180.f;
	params->range = .3f;
	params->speed = 5.f;
	params->color = { 234, 255, 0, 196 };
	params->endcol = { 128, 96, 96, 16 };

	shared_ptr<Emitter> jets = make_shared<Emitter>(getOwner());
	jets->initialize(resources->graphics.get(), params);
	getOwner()->addComponent(dynamic_pointer_cast<Component>(jets));
	jets->setActivation(false);
}

void UserInput::start() { }

void UserInput::finish() { }

std::shared_ptr<GameObject> UserInput::update()
{
	shared_ptr<RidgidBody> body = getOwner()->getComponent<RidgidBody>();

	if (body == nullptr) return nullptr;

	/////////////////////////////
	// ACTIONS BASED ON INPUTS //
	/////////////////////////////
	// get input component
	shared_ptr<UserInput> input = getOwner()->getComponent<UserInput>();

	if (input == nullptr) return nullptr;

	// adjust jet emitter
	shared_ptr<Emitter> jets = getOwner()->getComponent<Emitter>();
	Vector2D pos = body->getPosition();
	eFloat angle = body->getAngle();
	eFloat cosine = cosf(angle * DEG_TO_RAD);
	eFloat sine = sinf(angle * DEG_TO_RAD);
	Vector2D new_comp{ cosine, sine };
	jets->setPosition({pos.x + 5.f - cosine * 11.f, pos.y + 5.f - sine * 11.f });
	jets->setAngle(angle + 180.f);
	bool forward = input->inputStatus(InputHandler::Inputs::FORWARD);
	jets->setActivation(forward);
	if (forward) {
		if (!rumbling) {
			rumbling = true;
			resources->sounds->playSound(string("rumble_01.ogg"), -1, 5);
		}
		eFloat imp = speed * body->density;
		// move forward
		body->applyImpulse(Vector2D{ new_comp.x * imp, new_comp.y * imp });

		Vector2D current_vel = body->getVelocity();
		float direction = atan2f(current_vel.y, current_vel.x);
		Vector2D current_comp{ cosf(direction), sinf(direction) };

		float mag = sqrtf(powf(current_vel.x, 2) + powf(current_vel.y, 2));
		float citation = speedLimit - mag;
		if (citation < 0)
			body->applyImpulse(Vector2D{current_comp.x * citation , current_comp.y * citation });
	}
	else if (rumbling) {
		rumbling = false;
		resources->sounds->stopChannel(5);
	}
	if (input->inputStatus(InputHandler::Inputs::LEFT))
		body->setRotation(-turnSpeed);
	else if (input->inputStatus(InputHandler::Inputs::RIGHT))
		body->setRotation(turnSpeed);
	else body->setRotation(0.f);
	
	std::shared_ptr<GameObject> newObject;

	if (input->inputStatus(InputHandler::Inputs::PRIMARY)) {
		shared_ptr<PeaShooter> pew = getOwner()->getComponent<PeaShooter>();
		if (pew != nullptr) {
			shared_ptr<GameObject> bullet = pew->fire();
			if (bullet != nullptr) newObject = bullet;
		}
	}
	if (input->inputStatus(InputHandler::Inputs::MISSILE)) {
		shared_ptr<MissileLauncher> ml = getOwner()->getComponent<MissileLauncher>();
		if (ml != nullptr) {
			shared_ptr<GameObject> missile = ml->fire();
			if (missile != nullptr) newObject = missile;
		}
	}
	return newObject;
}

bool UserInput::inputStatus( InputHandler::Inputs key )
{
	return resources->inputs->keyStates[ key ];
}
