#include "GameObject.h"
#include "Texture.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "RidgidBodyComponent.h"
#include "InputHandler.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	target = nullptr;
	origin = nullptr;
	vector<std::shared_ptr<Component>> temp;
	components.clear();
	components.swap(temp);
}

bool GameObject::initialize()
{
	//Call the start method
	if (!initialized)
	{
		for (auto comp : components)
		{
			comp->start();
		}
		// activate physics
		shared_ptr<RidgidBody> body = getComponent<RidgidBody>();
		if (body != nullptr)body->setActive(true);

		initialized = true;
	}

	return(initialized);
}

std::shared_ptr<Component> GameObject::addComponent( shared_ptr<Component> component)
{
	components.push_back(component);
	return component;
}

std::shared_ptr<GameObject> GameObject::update()
{
	if (initialized) {
		pass_through = nullptr;
		for (auto component : components)
		{
			std::shared_ptr<GameObject> spawn;
			spawn = component->update();
			if (spawn != nullptr && pass_through == nullptr) {
				pass_through = spawn;
			}
		}
	}
	if (target != nullptr && !target->live)target = nullptr;
	if (origin != nullptr && !origin->live)origin = nullptr;
	return pass_through;
}

void GameObject::draw()
{
	if (initialized) {
		shared_ptr<Sprite> sprite = getComponent<Sprite>();
		if (sprite != nullptr) sprite->draw();
	}
}

std::shared_ptr<GameObject> GameObject::getAggro()
{
	return aggro.lock();
}

