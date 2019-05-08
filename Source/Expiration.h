#ifndef EXPIRATION_H
#define EXPIRATION_H

#include "Component.h"

class ObjectTemplate;

class Expire : public Component {
public:
	Expire(std::shared_ptr<GameObject>);
	~Expire();

	void start();
	void finish();
	void initialize(ObjectTemplate*);
	std::shared_ptr<GameObject> update();

private:
	Uint32 the_end;
	bool counting{ false };
	void RIP();
};

#endif // !EXPIRATION_H
