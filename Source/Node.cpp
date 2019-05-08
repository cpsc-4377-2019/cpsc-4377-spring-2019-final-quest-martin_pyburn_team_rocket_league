#include "GameObject.h"
#include "Node.h"
#include "ObjectDefinitions.h"

Node::Node(GameObject* owner) : _owner(owner) { }

Node::~Node() {}

void Node::update()
{

}


void Node::addNode(Node* node)
{

	child.push_back(node);
}