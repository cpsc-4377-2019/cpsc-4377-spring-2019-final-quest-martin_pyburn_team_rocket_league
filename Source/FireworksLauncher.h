#ifndef FIREWORKSLAUNCHER_H
#define FIREWORKSLAUNCHER_H

#include "Component.h"
#include "Definitions.h"

class ParticleParams;
class GameObject;
class GraphicsDevice;
class ObjectFactory;

class FWLauncher : public Component {
public:
	FWLauncher(std::shared_ptr<GameObject>);
	~FWLauncher();

	bool initialize(ObjectFactory*, GraphicsDevice*);

	void start();
	std::shared_ptr<GameObject> update();
	void finish();

	void draw();

private:
	GraphicsDevice* gDevice;
	ObjectFactory* factory;
	int delay{ 0 };
};

#endif // !FIREWORKSLAUNCHER_H