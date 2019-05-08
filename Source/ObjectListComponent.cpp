#include "ObjectListComponent.h"
#include "GameObject.h"

ObjectList::ObjectList(std::shared_ptr<GameObject> owner) : Component(owner)
{

}

ObjectList::~ObjectList()
{
}

void ObjectList::initialize()
{
}

void ObjectList::start()
{
}

std::shared_ptr<GameObject> ObjectList::update()
{
	return nullptr;
}

void ObjectList::finish()
{
}
