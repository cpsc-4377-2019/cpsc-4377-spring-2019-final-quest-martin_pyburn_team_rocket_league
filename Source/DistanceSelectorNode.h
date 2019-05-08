#ifndef DISTANCESELECTORNODE_H
#define DISTANCESELECTORNODE_H
#include "Node.h"

class DistanceSelector :public Node
{
public:
	DistanceSelector(GameObject*);
	~DistanceSelector();
	void update();
	void setChaseRadius(eFloat);
	void setFleeRadius(eFloat);

private:
	eFloat chase{ 0.f };
	eFloat flee{ 200.f };


};
#endif // !DISTANCESELECTORNODE_H
