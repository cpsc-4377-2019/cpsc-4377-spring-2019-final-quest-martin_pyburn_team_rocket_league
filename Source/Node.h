#ifndef NODE_H
#define NODE_H
#include "ObjectDefinitions.h"
#include "Definitions.h"
#include "RidgidBodyComponent.h"
#include "Component.h"
#include<vector>
class Node
{
public:
	Node(GameObject* owner);
	~Node();
	enum nodeType{LEAF, SEQUENCE, SELECTOR, NA};
	virtual void update();
	void addNode(Node* node);


protected:

	nodeType type;

	vector<Node*> child;
	GameObject* _owner;
};


#endif // !NODE_H

