#ifndef OBJECTLISTCOMPONENT_H
#define OBJECTLISTCOMPONENT_H

#include "Definitions.h"
#include "Component.h"

class GameObject;

class ObjectTemplate;

class ObjectList : public Component {

public:
	ObjectList(std::shared_ptr<GameObject>);
	~ObjectList();
	void initialize(shared_ptr<resource_map> resources, ObjectTemplate* temp);

	void start();
	std::shared_ptr<GameObject> update();
	void finish();

	std::vector<shared_ptr<GameObject>> list;
private:
};

#endif // !OBJECTLISTCOMPONENT_H
