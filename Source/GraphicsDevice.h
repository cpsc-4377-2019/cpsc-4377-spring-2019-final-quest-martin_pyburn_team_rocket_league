#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H
#include "Texture.h"




using namespace std;

class GraphicsDevice
{

public:
	GraphicsDevice(int w, int h);
	~GraphicsDevice();
	int getWidth();
	int getHeight();

	SDL_Renderer* getRenderer();

	void begin();

	void present();

	int FPS = 60;

private:

	//
	// PRIVATE METHODS
	//
	void initGraphicsDevice(int width = 800, int height = 800);

	void cleanUp();

	//
	//PRIVATE MEMBERS
	//
	bool quit;

	int width = 800;
	int height = 800;

	//Window(s) to display graphics
	SDL_Window* window;

	//Hardware accelerated renderer
	SDL_Renderer* renderer;
};
#endif