#ifndef INTEGRITY_H
#define INTEGRITY_H

#include<string>
#include<map>
#include<iostream>
#include "Component.h"
#include "SDL.h"

struct ObjectParams;

class Integrity : public Component
{
public:
	Integrity(std::shared_ptr<GameObject>);
	~Integrity();
	bool initialize(ObjectParams*);
	void start();
	std::shared_ptr<GameObject> update();
	void finish();

	void damage(float);
	
	float integrity{ 0 };
};


#endif // !INTEGRITY_H