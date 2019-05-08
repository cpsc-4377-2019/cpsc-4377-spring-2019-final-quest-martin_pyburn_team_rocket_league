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
	void initialize();

	void start();
	std::shared_ptr<GameObject> update();
	void finish();

	std::vector<shared_ptr<GameObject>> list;
private:
};

#endif // !OBJECTLISTCOMPONENT_H
