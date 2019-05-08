#include "Texture.h"
#include "GraphicsDevice.h"

using namespace std;


Texture::~Texture()
{
	if (sprite != nullptr)
	{
		SDL_DestroyTexture(sprite);
		sprite = nullptr;
	}
}


Texture::Texture(GraphicsDevice* gDevice, const std::string &path)
{
	// Load the image
	this->gDevice = gDevice;
	this->renderer = gDevice->getRenderer();
	SDL_Surface* surface = IMG_Load(path.c_str());

	// If the image failed to load
	if (surface == nullptr)
	{
		printf("Unable to load image %s!\n\tSDL_image Error: %s\n", path.c_str(), IMG_GetError());
		surface = IMG_Load("image_load_fail.png");

	}
	if (surface != nullptr)
	{
		// Create an optimized image
		sprite = SDL_CreateTextureFromSurface(renderer, surface);
		initialized = sprite != nullptr;
		// Free the loaded image; no longer needed
		SDL_FreeSurface(surface);

		//set the height and width from the texture
		SDL_QueryTexture(sprite, NULL, NULL, &width, &height);
	}

}

void Texture::draw( Vector2D& position, float angle, shared_ptr<Vector2D> centerPoint, shared_ptr<SDL_Rect> clip, int winW, int winH )
{
	if ( &sprite != nullptr )
	{
		if (winW == 0)winW = gDevice->getWidth();
		if (winH == 0)winH = gDevice->getHeight();

		//set rendering space and render to screen
		SDL_Rect renderQuad = { (int)roundf(position.x), (int)roundf(position.y), width, height };
		SDL_Point center;
		if (centerPoint == nullptr)
			center = { width / 2, height / 2 };
		else center = { (int)centerPoint->x, (int)centerPoint->y };
		//set clip rendering dimensions
		if (clip != nullptr) {
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		//Render to screen
		SDL_RenderCopyEx( renderer, sprite, clip.get(), &renderQuad, angle, &center, SDL_FLIP_NONE );

		bool h = position.x + renderQuad.w > winW;
		bool v = position.y + renderQuad.h > winH;

		if( h ) {
			// mirror horizontally
			renderQuad.x -= winW;
			SDL_RenderCopyEx( renderer, sprite, clip.get(), &renderQuad, angle, &center, SDL_FLIP_NONE );
		}
			// mirror vertically and/or diagonally depending on the situation
		if ( v ) {
			renderQuad.y -= winH;
			SDL_RenderCopyEx( renderer, sprite, clip.get(), &renderQuad, angle, &center, SDL_FLIP_NONE );
			if( h ) {
				renderQuad.x += winW;
				SDL_RenderCopyEx(renderer, sprite, clip.get(), &renderQuad, angle, &center, SDL_FLIP_NONE);
			}
		}
	}
}

void Texture::setRGBA(RGBA rgba)
{
	// apply color mod
	SDL_SetTextureColorMod(sprite, rgba.R, rgba.G, rgba.B);

	// apply alpha mod
	SDL_SetTextureAlphaMod(sprite, rgba.A);
}

void Texture::setWH(int w, int h) {
	width = w;
	height = h;
}
