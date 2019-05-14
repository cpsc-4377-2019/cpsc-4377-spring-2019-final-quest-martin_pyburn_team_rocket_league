#ifndef MISSILELAUNCHER_H
#define MISSILELAUNCHER_H

#include "Component.h"
#include "Definitions.h"
#include <vector>
#include <memory>

class GameObject;
class ObjectFactory;
class ObjectTemplate;

class MissileLauncher : public Component {
public:
	MissileLauncher(std::shared_ptr<GameObject>);
	~MissileLauncher();
	void start();
	void finish();
	void initialize(std::shared_ptr<resource_map> resources, ObjectTemplate* temp);
	std::shared_ptr<GameObject> update();

	std::shared_ptr<GameObject> fire();
	int getAmmoCount();

	GameObject* aggro;

private:
	float launch_speed = 0.f;
	float missile_damage = 150.f;

	std::shared_ptr<ObjectTemplate> missileTemplate;
	int fire_interval = 600;	// milliseconds
	int capacity = 5;
	int ammo = 0;

	int reload_interval = 1800;
	int last_reload = 0;
	int last_fired = 0;
	std::shared_ptr<resource_map> resources;

};

#endif // !MISSILELAUNCHER_H
