#ifndef EXPIRATION_H
#define EXPIRATION_H

#include "Component.h"
#include "Definitions.h"

class ObjectTemplate;
class ObjectFactory;
class GraphicsDevice;
class SoundDevice;

class Expire : public Component {
public:
	Expire(std::shared_ptr<GameObject>);
	~Expire();

	void start();
	void finish();
	void initialize(std::shared_ptr<resource_map> resources, ObjectTemplate* temp);
	std::shared_ptr<GameObject> update();

private:
	std::shared_ptr<resource_map> resources;
	Uint32 the_end;
	bool counting{ false };
	bool dead{ false };
	std::shared_ptr<GameObject> spawnEmitter();
};

#endif // !EXPIRATION_H
