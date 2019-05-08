#include "Object.h"
#include "Texture.h"
#include "Definitions.h"
#include "tinyxml2.h"

void Object::initBody( BodyParams bp )
{
	position = bp.position;
	velocity = bp.velocity;
}

void Object::initSprite(SpriteParams sp)
{
	sourceX = sp.sourceX;
	sourceY = sp.sourceY;
	sourceW = sp.sourceW;
	sourceH = sp.sourceH;
	srcRect = SDL_Rect{ (int)sourceX, (int)sourceY, (int)sourceW, (int)sourceH };
}

Position2D Object::getPosition()
{
	return Position2D();
}

void Object::setPosition(Position2D pos)
{
	this->position = pos;
}

void Object::setVelocity(Position2D vel) { velocity = vel; }

void Object::setCenter(Vector2D center) {
	this->center = center;
}

void Object::setPoints(vector<Vector2D> points) {
	this->points = points;
}

eFloat Object::pos_mod(eFloat a, eFloat b) {
	return fmod(fmod(a, b) + b, b);
}

void Object::attachImage(shared_ptr<Texture> img)
{
	this->img = img;
}