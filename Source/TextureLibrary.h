#ifndef TEXTURELIBRARY_H
#define TEXTURELIBRARY_H
#include<string>
#include<memory>
#include<map>

struct SDL_Renderer;
class GraphicsDevice;
class Texture;
struct resource_map;

class TextureLibrary {
public:
	TextureLibrary(std::shared_ptr<resource_map> resources);
	std::shared_ptr<Texture> getArtAsset(std::string name);

	std::string imgPath = "";

private:
	std::shared_ptr<resource_map> resources;
	std::map<std::string, std::shared_ptr<Texture>> artLibrary;
	GraphicsDevice* gDevice;
	SDL_Renderer* renderer;
};
#endif