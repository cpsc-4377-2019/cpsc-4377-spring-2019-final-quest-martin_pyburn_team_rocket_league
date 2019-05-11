#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL.h"
#include "SDL_image.h"
#include "Definitions.h"
class GraphicsDevice;
class Texture {

public:

	Texture() = delete;
	Texture(GraphicsDevice* gDevice, const std::string &path);
	~Texture();

	void draw(Vector2D& position, float angle = 0.0f, shared_ptr<Vector2D> centerPoint = nullptr, shared_ptr<SDL_Rect> clip = nullptr, eInt winW = 0, eInt winH = 0, eInt red = 255, eInt green = 255, eInt blue = 255, eInt alpha = 255);
	
	int width{ 0 };
	int height{ 0 };

	bool initialized{ false };

	void setWH(int w, int h);

private:
	//The actual hardware texture. created and destroyed by SDL
	SDL_Texture * sprite{ nullptr };

	GraphicsDevice* gDevice;
	SDL_Renderer* renderer;

};

#endif