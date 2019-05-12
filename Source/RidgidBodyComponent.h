#ifndef RIDGIDBODY_H
#define RIDGIDBODY_H

#include "Component.h"
#include "Initializers.h"

class GameObject;
class GameLibrary;
class Texture;
class PhysicsDevice;
struct SDL_Renderer;
struct Joints;

class RidgidBody : public Component
{
public:
	RidgidBody(std::shared_ptr<GameObject>);
	~RidgidBody();
	void initialize(SDL_Renderer* renderer, PhysicsDevice* physics, ObjectTemplate* temp);

	void start();
	std::shared_ptr<GameObject> update();
	void finish();

	Vector2D getVelocity();
	Vector2D getPosition();
	Vector2D getCentroid();
	eFloat getAngle();
	eFloat getRotation();
	eFloat getArea();

	float getRelativeAngle(Vector2D);
	float getDirection(Vector2D);
	float getDistance(Vector2D);

	bool setType(BodyType);

	bool setVelocity(Vector2D);
	bool setTransform(Vector2D, eFloat);
	bool setAngle(eFloat);
	bool setRotation(eFloat);
	bool applyImpulse(Vector2D);
	bool accelerate(Vector2D force);
	bool applyTorque(eFloat);
	bool setActive(bool);
	bool setFilterIndex(int);

	bool joinWith(RidgidBody*, Joints*);
	
	eFloat density{ 1.f };
	Vector2D center;
	int layer{ 0 };
	bool shell{ false };

	std::shared_ptr<vector<float>> points;
protected:

	// used to get width and height of play area
	SDL_Renderer* renderer;

	PhysicsDevice* physics;

	eFloat pos_mod(eFloat a, eFloat b) {
		return fmod(fmod(a, b) + b, b);
	}
};


#endif