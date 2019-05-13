#ifndef FRAGBEHAVIOR_H
#define FRAGBEHAVIOR_H

#include "GameObject.h"
#include "Component.h"
#include "Definitions.h"

#include "SDL.h"

class GraphicsDevice;
class SoundDevice;
class ObjectFactory;

class Frag : public Component {
public:
	Frag(std::shared_ptr<GameObject>);
	~Frag();
	void initialize(shared_ptr<resource_map> resources, ObjectTemplate* temp);
	void start();
	std::shared_ptr<GameObject> update();
	void finish();

private:
	shared_ptr<resource_map> resources;
};

#endif // !FRAGBEHAVIOR_H