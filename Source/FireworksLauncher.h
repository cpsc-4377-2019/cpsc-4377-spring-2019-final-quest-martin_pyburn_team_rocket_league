#ifndef FIREWORKSLAUNCHER_H
#define FIREWORKSLAUNCHER_H

#include "Component.h"
#include "Definitions.h"

class ParticleParams;
class GameObject;
class GraphicsDevice;
class ObjectFactory;
class ObjectTemplate;

class FWLauncher : public Component {
public:
	FWLauncher(std::shared_ptr<GameObject>);
	~FWLauncher();

	void initialize(std::shared_ptr<resource_map> resources, ObjectTemplate* temp = nullptr);

	void start();
	std::shared_ptr<GameObject> update();
	void finish();

	void draw();

private:
	std::shared_ptr<resource_map> resources;
	int delay{ 0 };
};

#endif // !FIREWORKSLAUNCHER_H