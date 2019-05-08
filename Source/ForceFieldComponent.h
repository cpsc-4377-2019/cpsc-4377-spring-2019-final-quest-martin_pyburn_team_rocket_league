#ifndef FORCEFIELD_H
#define FORCEFIELD_H

#include "Component.h"

class ObjectFactory;
struct ObjectParams;

class ForceField : public Component {
public:
	ForceField(std::shared_ptr<GameObject>);
	~ForceField();

	bool initialize(ObjectFactory*, ObjectParams*);

	void start();
	std::shared_ptr<GameObject> update();
	void finish();

private:
	std::shared_ptr<ObjectTemplate> fieldTemplate;
	ObjectFactory* factory;
	bool ready{ false };
};

#endif // !FORCEFIELD_H
