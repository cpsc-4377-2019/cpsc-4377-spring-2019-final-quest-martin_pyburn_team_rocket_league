#include "GraphicsDevice.h"


GraphicsDevice::GraphicsDevice()
{
	window = nullptr;

	initGraphicsDevice();

}

GraphicsDevice::~GraphicsDevice()
{
	cleanUp();

}

int GraphicsDevice::getWidth() { return width; }
int GraphicsDevice::getHeight() { return height; }

SDL_Renderer * GraphicsDevice::getRenderer()
{
	return renderer;
}

void GraphicsDevice::begin() {
	SDL_RenderClear(renderer);
}

void GraphicsDevice::present()
{	
	SDL_RenderPresent(renderer);
}

void GraphicsDevice::initGraphicsDevice(int width, int height)
{
	this->width = width;
	this->height = height;
	//Initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	//initialize SDL_image subsystems
	if (!IMG_Init(IMG_INIT_PNG))
	{
		printf("SDL_image could not initialize! SDL_Error: %s\n", IMG_GetError());
	}

	//Construct and check window construction
	window = SDL_CreateWindow("Planetoids",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	//Construct and check renderer construction
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	//set the background color (default)
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void GraphicsDevice::cleanUp() {


	//Free the window
	SDL_DestroyWindow(window);
	window = NULL;

	//Free the renderer
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	IMG_Quit();
}