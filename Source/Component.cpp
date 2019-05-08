#include "GameObject.h"
#include "Component.h"
#include "ObjectDefinitions.h"

Component::Component(std::shared_ptr<GameObject> owner) : _owner( owner ) { }

Component::~Component() {
	_owner.reset();
}

void Component::ownerDestroyed()
{
	finish();
}


std::shared_ptr<GameObject> Component::getOwner()
{
	return _owner.lock();
}