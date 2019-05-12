#ifndef EXPIRATION_H
#define EXPIRATION_H

#include "Component.h"

class ObjectTemplate;
class ObjectFactory;
class GraphicsDevice;

class Expire : public Component {
public:
	Expire(std::shared_ptr<GameObject>);
	~Expire();

	void start();
	void finish();
	void initialize(ObjectFactory* factory, GraphicsDevice* gDevice, ObjectTemplate*);
	std::shared_ptr<GameObject> update();

private:
	GraphicsDevice* gDevice;
	ObjectFactory* factory;
	Uint32 the_end;
	bool counting{ false };
	bool dead{ false };
	std::shared_ptr<GameObject> spawnEmitter();
};

#endif // !EXPIRATION_H
