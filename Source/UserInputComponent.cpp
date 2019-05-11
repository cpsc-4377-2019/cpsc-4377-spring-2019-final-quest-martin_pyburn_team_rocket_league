#include "UserInputComponent.h"
#include "InputHandler.h"
#include "GraphicsDevice.h"
#include "GameObject.h"
#include "RidgidBodyComponent.h"
#include "PeaShooter.h"
#include "MissileLauncher.h"
#include "ParticleEmitter.h"

UserInput::UserInput(std::shared_ptr<GameObject> owner) : Component( owner ) { }

UserInput::~UserInput() { }

void UserInput::initialize(GraphicsDevice* gDevice, InputHandler* input, string path)
{
	this->input = input;
	this->path = path;
	shared_ptr<ParticleParams> params = make_shared<ParticleParams>();
	params->texture = "./Assets/Images/puff.png";
	params->ppf = 1;
	params->lifespan = -1;
	params->parttime = 15;
	params->rx = 0;
	params->ry = 0;
	params->rw = 5;
	params->rh = 5;
	params->cx = 2.5f;
	params->cy = 2.5f;
	params->angle = 180.f;
	params->range = 1.f;
	params->speed = 300.f;
	params->color = { 234, 255, 0, 196 };
	params->endcol = { 128, 96, 96, 16 };

	shared_ptr<Emitter> jets = make_shared<Emitter>(getOwner());
	jets->initialize(gDevice, params);
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
	if (input->inputStatus(InputHandler::Inputs::FORWARD)) {

		eFloat angle = body->getAngle();

		shared_ptr<Emitter> jets = getOwner()->getComponent<Emitter>();
		jets->setPosition(body->getPosition());
		jets->setActivation(angle + 180.f);
		jets->setActivation(true);

		eFloat imp = speed * body->density;
		// move forward
		Vector2D new_comp{ cosf(angle * DEG_TO_RAD), sinf(angle * DEG_TO_RAD) };
		body->applyImpulse(Vector2D{ new_comp.x * imp, new_comp.y * imp });

		Vector2D current_vel = body->getVelocity();
		float direction = atan2f(current_vel.y, current_vel.x);
		Vector2D current_comp{ cosf(direction), sinf(direction) };

		float mag = sqrtf(powf(current_vel.x, 2) + powf(current_vel.y, 2));
		float citation = speedLimit - mag;
		if (citation < 0)
			body->applyImpulse(Vector2D{current_comp.x * citation , current_comp.y * citation });

		//Vector2D newVel = *body->velocity + Vector2D({ new_comp.x * speed, new_comp.y * speed });
		//float mag = sqrtf(powf(newVel.x, 2) + powf(newVel.y, 2));
		//if (mag >= speedLimit) {
		//	body->velocity->x -= current_comp.x * speed;
		//	body->velocity->y -= current_comp.y * speed;
		//}
		//body->velocity->x += new_comp.x * speed;
		//body->velocity->y += new_comp.y * speed;
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
	return input->keyStates[ key ];
}
