#ifndef PHYSICSDEVICE_H
#define PHYSICSDEVICE_H

#include <memory>
#include "Box2D.h"
#include "Definitions.h"
#include "Initializers.h"
#include "ContactListener.h"
#include "B2Debug.h"

class Texture;
class RidgidBody;

class PhysicsDevice {
public:
	//Tricky trying to get initialize into one of the constructors.
	PhysicsDevice() = delete;
	~PhysicsDevice();
	PhysicsDevice(Vector2D gravity, GraphicsDevice* gDevice);

	bool update(float dt);
	bool createFixture(RidgidBody* object, BodyPresets presets);
	bool createJoint(Joints joint);
	bool createBoundary(RidgidBody* object, int x, int y, int w, int h);
	bool removeObject(RidgidBody* object);

	bool setBodyType(const RidgidBody*, BodyType);
	bool setGravity(Vector2D gravity);
	bool setTransform(const RidgidBody* object, Vector2D position, eFloat angle);
	bool setLinearVelocity(const RidgidBody* object, Vector2D linearVelociy);
	bool setAngularVelocity(const RidgidBody* object, eFloat angularVelocity);
	bool setTorque(const RidgidBody* object, eFloat torque);
	bool setLinearImpulse(const RidgidBody* object, Vector2D forceVec, Vector2D forceCenter);
	bool setStatic(const RidgidBody* object);
	bool setStopPhysics(const RidgidBody* object);
	bool setAngle(const RidgidBody* object, eFloat angle);
	bool accelerate(const RidgidBody* object, Vector2D forceVec);
	bool setActive(const RidgidBody * object, bool active);
	bool setFilterIndex(const RidgidBody*, int);

	void debugDraw();

	eFloat getAngularVelocity(const RidgidBody* object) const;
	Vector2D getPosition(const RidgidBody* object) const;
	eFloat getAngle(const RidgidBody* object) const;
	eFloat getArea(const RidgidBody* object) const;
	Vector2D getVelocity(const RidgidBody* object) const;
	Vector2D getLinearVelocity(const RidgidBody* object) const;
	bool getLive(const RidgidBody* object) const;

	// need for debug
	inline float PW2RW(eFloat x)const { return x * fPRV; }

private:

	b2Body* FindBody(const RidgidBody* RidgidBody) const;

	inline float RW2PW(eFloat x)const { return x / fPRV; }
	inline float RW2PW(eInt x)const { return (float)x / fPRV; }
	inline float RW2PWAngle(eFloat x)const { return((float)x*(2.0f*3.14159f) / 360.0f); } //degrees to radians
	inline float PW2RWAngle(eFloat x)const { return((float)x*360.0f / (2.0f*3.14159f)); } //radians to degrees
	b2Vec2 GV2PV(Vector2D gameVec)const;
	Vector2D PV2GV(b2Vec2 physicsVec)const;
	Vector2D alignCenters(const  RidgidBody* object)const;


	bool destroyJoint(RidgidBody* object);
	bool destroyJoint(b2Body* body);

	unique_ptr<b2World> world{ nullptr };
	const b2Vec2 gravity{ 0,0 };
	static const float fPRV;

	std::shared_ptr<B2Debug> B2Debugger;
	
};
#endif