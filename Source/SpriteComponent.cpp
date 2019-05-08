#include "TextureLibrary.h"
#include "GameObject.h"
#include "Component.h"
#include "RidgidBodyComponent.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include <string>

Sprite::Sprite(std::shared_ptr<GameObject> owner) : Component(owner)
{
}

Sprite::~Sprite() {}

void Sprite::initialize(TextureLibrary* library, ObjectTemplate* temp)
{
	this->library = library;
	//Load the Art Asset
	path = (*temp)[SPRITE]->path;
	rect = make_shared<SDL_Rect>(SDL_Rect{
		(int)(*temp)[SPRITE]->sx, (int)(*temp)[SPRITE]->sy,
		(int)(*temp)[SPRITE]->sw, (int)(*temp)[SPRITE]->sh });
	*center = { (*temp)[BODY]->cx, (*temp)[BODY]->cy };
	if ((*temp)[SPRITE]->colorSet)
		color.set((*temp)[SPRITE]->color);
	if(path != "")setTexture(&path);
	ready = true;
}

void Sprite::start() {}

std::shared_ptr<GameObject> Sprite::update() {
	return nullptr;
}

void Sprite::finish() {}

void Sprite::draw()
{
	if (ready) {
		shared_ptr<RidgidBody> body = getOwner()->getComponent<RidgidBody>();
		if (body != nullptr) {
			texture->draw(body->getPosition(), body->getAngle(), center, rect);
		}
	}
}

void Sprite::setTexture(string * path)
{
	this->path = *path;
	texture = library->getArtAsset(library->imgPath + *path);
	texture->setRGBA(color);
}

shared_ptr<Texture> Sprite::getTexture()
{
	return texture;
}

string Sprite::getPath()
{
	return path;
}
