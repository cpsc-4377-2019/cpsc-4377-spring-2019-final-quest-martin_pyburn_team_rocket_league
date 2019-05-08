#include "GameObject.h"
#include "Expiration.h"
#include "ObjectDefinitions.h"
#include <chrono>

using namespace chrono;

Expire::Expire(std::shared_ptr<GameObject> owner) : Component( owner )
{
}

Expire::~Expire()
{
}

void Expire::start()
{
	the_end += SDL_GetTicks();
	counting = true;
}

void Expire::finish()
{
}

void Expire::initialize(ObjectTemplate* temp )
{
	if( (*temp)[ EXPIRE ]->set )
		the_end = (long long)(*temp)[ EXPIRE ]->value;
}

std::shared_ptr<GameObject> Expire::update()
{
	if (counting && SDL_GetTicks() >= the_end ) getOwner()->live = false;
	return nullptr;
}

void Expire::RIP()
{
}
