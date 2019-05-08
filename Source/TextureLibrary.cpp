#include "TextureLibrary.h"
#include "GraphicsDevice.h"
#include"Texture.h"

TextureLibrary::TextureLibrary(GraphicsDevice* gDevice)
{
	this->gDevice = gDevice;
	this->renderer = gDevice->getRenderer();
}

shared_ptr<Texture> TextureLibrary::getArtAsset(std::string name)
{
	auto it = artLibrary.find(name);
	if (it == artLibrary.end()) {
		artLibrary.insert_or_assign(name, make_shared<Texture>(gDevice, name));

		if (!artLibrary.find(name)->second->initialized)
		{
			auto artIter = artLibrary.find(name);
			artLibrary.erase(artIter);
		}
	}
	return artLibrary.find(name)->second;
}
