#include "GameObject.h"
#include "ObjectDefinitions.h"
#include "RidgidBodyComponent.h"
#include "GraphicsDevice.h"
#include "PhysicsDevice.h"
#include <string>

RidgidBody::RidgidBody(std::shared_ptr<GameObject> owner) : Component(owner)
{
}

RidgidBody::~RidgidBody() {
	//remove the physics body
	if (!physics->removeObject(this))
	{
		printf("Object could not be removed from Physics World");
		exit(1);
	}
}


void RidgidBody::initialize(shared_ptr<resource_map> resources, ObjectTemplate* temp)
{
	this->physics = resources->physics.get();
	this->renderer = resources->graphics->getRenderer();

	Vector2D position;
	position.x = (*temp)[BODY]->x;
	position.y = (*temp)[BODY]->y;
	
	eFloat angle = (*temp)[BODY]->angle * RAD_TO_DEG;
	
	Vector2D velocity;
	velocity.x = (*temp)[BODY]->vx;
	velocity.y = (*temp)[BODY]->vy;
	
	float vAngle = (*temp)[BODY]->vangle;
	
	center.x = (*temp)[BODY]->cx;
	center.y = (*temp)[BODY]->cy;

	layer = (*temp)[BODY]->layer;
	if (getOwner()->type == objectTypes::PLANETOID)layer = 2;
	shell = (*temp)[BODY]->shell;
	density = (*temp)[BODY]->density;

	PhysicsStats phys;
	phys.spinSpeed = (*temp)[BODY]->vangle;
	phys.poly = (*temp)[BODY]->points;
	phys.density = density;
	phys.filter = (*temp)[BODY]->layer;
	phys.linearDamping = (*temp)[BODY]->ldamp;
	phys.angularDamping = (*temp)[BODY]->adamp;

	phys.poly = (*temp)[BODY]->points;
	// polygon body
	if (phys.poly.size())
		phys.bodyShape = BodyShape::Polygon;
	else { // circle body
		phys.bodyShape = BodyShape::Circle;
		phys.radius = (*temp)[BODY]->radius;
		if (phys.radius == 0.0f) phys.radius = max(center.x, center.y);
	}
	
	BodyPresets presets{
		Vector2D{
			(*temp)[BODY]->x,
			(*temp)[BODY]->y
		},
		(*temp)[BODY]->angle * RAD_TO_DEG,
		phys
	};

	physics->createFixture(this, presets);
	physics->setLinearVelocity(this, velocity);
	physics->setTorque(this, vAngle);
}

void RidgidBody::start() {}

std::shared_ptr<GameObject> RidgidBody::update() {

	return nullptr;
}

void RidgidBody::finish() {}

Vector2D RidgidBody::getVelocity()
{
	return physics->getVelocity(this);
}

Vector2D RidgidBody::getPosition()
{
	return physics->getPosition(this);
}

Vector2D RidgidBody::getCentroid()
{
	return center;
}

eFloat RidgidBody::getAngle()
{
	float angle = physics->getAngle(this);
	float fullc = 360.f;
	return fmodf(fullc + fmodf(angle, fullc), fullc);
}

eFloat RidgidBody::getRotation()
{
	return physics->getAngularVelocity(this);
}

eFloat RidgidBody::getArea()
{
	return physics->getArea(this);
}

float RidgidBody::getRelativeAngle(Vector2D point)
{
	float angle = getAngle() * DEG_TO_RAD;
	float direction = getDirection(point);
	float difference = (angle - direction) * RAD_TO_DEG;
	if(difference < 180.f) return difference;
	else return difference - 360.f;
}

float RidgidBody::getDirection(Vector2D point)
{
	Vector2D pos = getPosition();
	return atan2f(point.y - pos.y, point.x - pos.x);
}

float RidgidBody::getDistance(Vector2D point)
{
	Vector2D pos = getPosition();
	return sqrtf(powf(pos.x - point.x, 2.f) + powf(pos.y - point.y, 2.f));
}

bool RidgidBody::setType(BodyType type) {
	return physics->setBodyType(this, type);
}

bool RidgidBody::setVelocity(Vector2D vel)
{
	return physics->setLinearVelocity(this, vel);
}

bool RidgidBody::setTransform(Vector2D pos, eFloat ang)
{
	return physics->setTransform(this, pos, ang);
}

bool RidgidBody::setAngle(eFloat angle)
{
	return physics->setAngle(this, angle);
}

bool RidgidBody::setRotation(eFloat vel)
{
	return physics->setAngularVelocity(this, vel);
}

bool RidgidBody::applyImpulse(Vector2D force)
{
	return physics->setLinearImpulse(this, force, Vector2D{ 0.f, 0.f });
}

bool RidgidBody::accelerate(Vector2D force) {
	return physics->accelerate(this, force);
}

bool RidgidBody::applyTorque(eFloat torque)
{
	return physics->setTorque(this, torque);
}

bool RidgidBody::setActive(bool active)
{
	return physics->setActive(this, active);
}

bool RidgidBody::setFilterIndex(int index)
{
	return physics->setFilterIndex(this, index);
}

bool RidgidBody::joinWith(RidgidBody* buddy, Joints* joint)
{
	joint->BodyA = this;
	joint->BodyB = buddy;
	return physics->createJoint(*joint);
}

