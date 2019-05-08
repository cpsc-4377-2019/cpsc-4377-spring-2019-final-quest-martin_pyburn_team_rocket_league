#include "IntegrityComponent.h"
#include "ObjectDefinitions.h"
#include "ObjectListComponent.h"
#include "GameObject.h"
#include "Component.h"

Integrity::Integrity(std::shared_ptr<GameObject> owner) : Component(owner)
{
}

Integrity::~Integrity()
{
}

bool Integrity::initialize(ObjectParams* params)
{
	integrity = params->value;
	return false;
}

void Integrity::start()
{
}

std::shared_ptr<GameObject> Integrity::update()
{
	if (integrity <= 0) {
		getOwner()->live = false;
	}
	return nullptr;
}

void Integrity::finish()
{
}

void Integrity::damage(float dmg) {
	getOwner()->distress = true;
	if (integrity > 0) integrity -= dmg;
}
