#include "TextureLibrary.h"
#include "GraphicsDevice.h"
#include "Texture.h"
#include "Definitions.h"

TextureLibrary::TextureLibrary(shared_ptr<resource_map> resources)
{
	this->resources = resources;
}

shared_ptr<Texture> TextureLibrary::getArtAsset(std::string name)
{
	name = resources->imgPath + name;
	auto it = artLibrary.find(name);
	if (it == artLibrary.end()) {
		artLibrary.insert_or_assign(name, make_shared<Texture>(resources->graphics.get(), name));

		if (!artLibrary.find(name)->second->initialized)
		{
			auto artIter = artLibrary.find(name);
			artLibrary.erase(artIter);
		}
	}
	return artLibrary.find(name)->second;
}
