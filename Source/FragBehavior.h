#ifndef FRAGBEHAVIOR_H
#define FRAGBEHAVIOR_H

#include "GameObject.h"
#include "Component.h"

#include "SDL.h"

class ObjectFactory;

class Frag : public Component {
public:
	Frag(std::shared_ptr<GameObject>);
	~Frag();
	void initialize(ObjectFactory* factory);
	void start();
	std::shared_ptr<GameObject> update();
	void finish();

private:
	ObjectFactory* factory;
};

#endif // !FRAGBEHAVIOR_H