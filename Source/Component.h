#ifndef COMPONENT_H
#define COMPONENT_H
#include <memory>
#include <vector>

class GameObject;
class ObjectTemplate;

union SDL_Event;

class Component
{
public:
	Component(std::shared_ptr<GameObject> owner);
	~Component();
	void ownerDestroyed();
	std::shared_ptr<GameObject> getOwner();
	virtual void start() = 0;
	virtual std::shared_ptr<GameObject> update() = 0;
	virtual void finish() = 0;
	void setActivation(bool active) { activated = active; };


	bool activated = true;
protected:
	std::weak_ptr<GameObject> _owner;
};


#endif