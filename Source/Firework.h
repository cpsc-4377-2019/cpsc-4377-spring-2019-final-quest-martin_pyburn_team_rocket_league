#ifndef FIREWORK_H
#define	FIREWORK_H

#include "Definitions.h"
#include "Component.h"

class GraphicsDevice;

class Firework : public Component{
public:
	Firework(std::shared_ptr<GameObject>);
	~Firework();

	bool initialize(GraphicsDevice* gDevice);

	void start();
	void finish();

	std::shared_ptr<GameObject> update();
	void draw();

private:
	GraphicsDevice* gDevice;

	RGBA randCol();
	eInt randVal();

	int age{ 0 };
	int burst{ 0 };
};
#endif // !FIREWORK_H
