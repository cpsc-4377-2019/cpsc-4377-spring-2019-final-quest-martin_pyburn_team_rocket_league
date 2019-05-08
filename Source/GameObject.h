#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<memory>
#include<iostream>
#include<vector>
#include "SDL.h"
#include "ObjectDefinitions.h"

class Component;
class Sprite;
class InputHandler;
class GameLibrary;

using namespace std;

class GameObject
{
public:
	GameObject();
	~GameObject();
	bool initialize();

	std::shared_ptr<Component> addComponent( shared_ptr<Component> component);

	template<class T>
	shared_ptr<T> getComponent()
	{
		for (auto comp : components)
		{
			shared_ptr<T> t = nullptr;
			if (t = dynamic_pointer_cast<T>(comp))
			{
				return t;
			}
		}

		return nullptr;
	}

	std::shared_ptr<GameObject> update();
	void draw();

	std::shared_ptr<GameObject> getAggro();
	
	// vitality
	bool live{ true };
	std::shared_ptr<GameObject> pass_through;

	int type{ objectTypes::NA };
	eFloat damage{ 0 };

	std::shared_ptr<GameObject> origin;
	std::shared_ptr<GameObject> target;
	std::weak_ptr<GameObject> aggro;

	bool initialized{ false };
	bool distress{ false };
protected:

	vector<shared_ptr<Component>> components;

};

#endif
