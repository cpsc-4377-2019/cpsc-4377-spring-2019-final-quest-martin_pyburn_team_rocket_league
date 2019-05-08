#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <map>

class InputHandler {
public:
	InputHandler();
	enum class Inputs
	{
		NA, FORWARD, LEFT, RIGHT, PRIMARY, MISSILE, PAUSE, QUIT, RELOAD, MAP, DEBUG
	};

	void update(SDL_Event* event);
	std::map<Inputs, bool> keyStates;

};

#endif // !INPUTHANDLER_H
