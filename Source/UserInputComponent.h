#ifndef USERINPUTCOMPONENT_H
#define USERINPUTCOMPONENT_H
#include "Component.h"
#include "InputHandler.h"
#include <memory>

class InputHandler;

class UserInput : public Component {
public:
	UserInput(std::shared_ptr<GameObject>);
	~UserInput();

	void initialize(InputHandler* input );

	void start();
	void finish();
	std::shared_ptr<GameObject> update();

	bool inputStatus( InputHandler::Inputs );

private:
	InputHandler* input;
	float speedLimit = 800.f;
	float speed = 8.f;
	float turnSpeed = 57.296f;
};

#endif // !USERINPUTCOMPONENT_H
