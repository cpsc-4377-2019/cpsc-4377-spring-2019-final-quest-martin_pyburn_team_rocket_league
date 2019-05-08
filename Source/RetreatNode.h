#ifndef RETREATNODE_H
#define RETREATNODE_H
#include "Node.h"

class Retreat :public Node
{
public:
	Retreat(GameObject*);
	~Retreat();
	void update();

private:
	int frames{ 0 };
};
#endif // !RETREATNODE_H
