#include "UserInputComponent.h"
#include "InputHandler.h"
#include "GameObject.h"
#include "RidgidBodyComponent.h"
#include "PeaShooter.h"
#include "MissileLauncher.h"

UserInput::UserInput(std::shared_ptr<GameObject> owner) : Component( owner ) { }

UserInput::~UserInput() { }

void UserInput::initialize(InputHandler* input)
{
	this->input = input;
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
