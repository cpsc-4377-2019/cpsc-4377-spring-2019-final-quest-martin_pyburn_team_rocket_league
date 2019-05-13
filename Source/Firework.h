#ifndef FIREWORK_H
#define	FIREWORK_H

#include "Definitions.h"
#include "Component.h"

class GraphicsDevice;

class Firework : public Component{
public:
	Firework(std::shared_ptr<GameObject>);
	~Firework();

	void initialize(std::shared_ptr<resource_map>, ObjectTemplate* temp = nullptr);

	void start();
	void finish();

	std::shared_ptr<GameObject> update();
	void draw();

private:
	std::shared_ptr<resource_map> resources;

	RGBA randCol();
	eInt randVal();

	int age{ 0 };
	int burst{ 0 };
};
#endif // !FIREWORK_H
