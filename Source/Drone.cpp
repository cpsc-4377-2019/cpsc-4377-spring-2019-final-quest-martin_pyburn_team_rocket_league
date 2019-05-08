#include "Drone.h"
#include "GameObject.h"
#include "ObjectFactory.h"
#include "RidgidBodyComponent.h"
#include "Box2D.h"
#include "PhysicsDevice.h"
#include "PeaShooter.h"
#include "Component.h"

Drone::Drone(std::shared_ptr<GameObject> owner) : Component( owner )
{
	shared_ptr<RidgidBody> obody = owner->getComponent<RidgidBody>();
	Vector2D opos = obody->getPosition();

	// construct drone template
	droneTemplate = make_shared<ObjectTemplate>();
	ObjectParams* droneBody = (*droneTemplate)[BODY].get();
	droneBody->layer = obody->layer;
	droneBody->cx = droneBody->cy = droneBody->radius = 8.f;
	droneBody->x = opos.x;
	droneBody->y = opos.y;
	droneBody->density = 0.01f;
	droneBody->set = true;
	ObjectParams* droneSprite = (*droneTemplate)[SPRITE].get();
	droneSprite->path = "drone.png";
	droneSprite->sx = droneSprite->sy = 0.f;
	droneSprite->sw = droneSprite->sh = 16.f;
	droneSprite->set = true;
	ObjectParams* droneDrone = (*droneTemplate)[DRONE].get();
	droneDrone->layer = 1;
	droneDrone->set = true;
	ObjectParams* droneShooter = (*droneTemplate)[PEASHOOTER].get();
	droneShooter->set = true;
	ObjectParams* droneAuto = (*droneTemplate)[AUTOFIRE].get();
	droneAuto->radius = 200.f;
	droneAuto->value = 10.f;
	droneAuto->set = true;
}

Drone::~Drone()
{
}

bool Drone::initialize(ObjectFactory * factory, ObjectTemplate* temp)
{
	this->factory = factory;
	if (!(*temp)[DRONE]->layer) master = true;

	return false;
}

void Drone::start() {}

std::shared_ptr<GameObject> Drone::update()
{
	if (master) {
		if (counterpart == nullptr) {
			// This is the ship component - build drone
			shared_ptr<GameObject> myDrone = make_shared<GameObject>();
			myDrone->origin = getOwner();
			factory->applyTemplate(droneTemplate, myDrone);

			myDrone->type = objectTypes::COMPONENT;

			// Link up master and drone
			counterpart = myDrone->getComponent<Drone>();
			counterpart->counterpart = getOwner()->getComponent<Drone>();

			shared_ptr<RidgidBody> shipBody = getOwner()->getComponent<RidgidBody>();
			shared_ptr<RidgidBody> droneBody = myDrone->getComponent<RidgidBody>();

			// attach drone with joint
			Joints joint;
			joint.type = JointType::Rope;
			joint.JointLimit = 20.f;
			joint.CollideConnected = false;
			joint.AnchorA = shipBody->center;
			joint.AnchorB = Vector2D{ 0.f,0.f };
			droneBody->joinWith(shipBody.get(), &joint);

			return myDrone;
		}
	}
	else if (counterpart != nullptr) {
		shared_ptr<GameObject> aggro = counterpart->getOwner()->getAggro();
		if (aggro != nullptr) {
			getOwner()->aggro = aggro;

			// face the aggro
			shared_ptr<RidgidBody> body = getOwner()->getComponent<RidgidBody>();
			shared_ptr<RidgidBody> aggroBody = aggro->getComponent<RidgidBody>();
			if (body != nullptr) {
				Vector2D aggroPos = aggroBody->getPosition();

				eFloat relAngle = body->getRelativeAngle(aggroPos);
				if (relAngle != 0.f) body->setRotation(-relAngle * 2);
			}
		}
	}
	else {
		getOwner()->live = false;
		return nullptr;
	}
	return nullptr;
}

void Drone::finish(){}
