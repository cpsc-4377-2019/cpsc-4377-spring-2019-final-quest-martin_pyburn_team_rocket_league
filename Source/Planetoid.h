#ifndef PLANETOID_H
#define PLANETOID_H
#include "object.h"
#include <vector>

struct ObjectParams;
struct Vector2D;

class Planetoid : public Object {
public:
	Planetoid( PlanetoidParams params );

	shared_ptr<vector<shared_ptr<Planetoid>>> damage(float dmg);

	void appendChild(shared_ptr<Planetoid> newChild);

	Object* update(float windowWidth, float windowHeight);

	void draw();

	string values();

	int index;

protected:
	// special Planetoid values
	float integrity;
	int layer;
	bool shell;

	shared_ptr<vector<shared_ptr<Planetoid>>> children = make_shared<vector<shared_ptr<Planetoid>>>();

};

#endif // !PLANETOID_H