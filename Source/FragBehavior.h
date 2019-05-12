#ifndef FRAGBEHAVIOR_H
#define FRAGBEHAVIOR_H

#include "GameObject.h"
#include "Component.h"

#include "SDL.h"

class GraphicsDevice;
class ObjectFactory;

class Frag : public Component {
public:
	Frag(std::shared_ptr<GameObject>);
	~Frag();
	void initialize(GraphicsDevice* gDevice, ObjectFactory* factory);
	void start();
	std::shared_ptr<GameObject> update();
	void finish();

private:
	GraphicsDevice* gDevice;
	ObjectFactory* factory;
};

#endif // !FRAGBEHAVIOR_H