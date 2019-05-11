#ifndef USERINPUTCOMPONENT_H
#define USERINPUTCOMPONENT_H
#include "Component.h"
#include "InputHandler.h"
#include <string>
#include <memory>

class InputHandler;
class GraphicsDevice;

class UserInput : public Component {
public:
	UserInput(std::shared_ptr<GameObject>);
	~UserInput();

	void initialize(GraphicsDevice*, InputHandler*, std::string);
	std::string path = "";

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
