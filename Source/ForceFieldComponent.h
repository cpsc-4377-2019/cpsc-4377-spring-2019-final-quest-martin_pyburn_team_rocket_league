#ifndef FORCEFIELD_H
#define FORCEFIELD_H

#include "Component.h"
#include "Definitions.h"

class ObjectFactory;
struct ObjectParams;

class ForceField : public Component {
public:
	ForceField(std::shared_ptr<GameObject>);
	~ForceField();

	void initialize(std::shared_ptr<resource_map>, ObjectTemplate*);

	void start();
	std::shared_ptr<GameObject> update();
	void finish();

private:
	std::shared_ptr<ObjectTemplate> fieldTemplate;
	ObjectFactory* factory;
	bool ready{ false };
};

#endif // !FORCEFIELD_H
