#include "ObjectListComponent.h"
#include "GameObject.h"

ObjectList::ObjectList(std::shared_ptr<GameObject> owner) : Component(owner)
{

}

ObjectList::~ObjectList()
{
}

void ObjectList::initialize(shared_ptr<resource_map> resources, ObjectTemplate* temp)
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
