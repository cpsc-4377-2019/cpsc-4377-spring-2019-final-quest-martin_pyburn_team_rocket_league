#ifndef PHYSICSDEVICE
#define PHYSICSDEVICE

#include "PhysicsDevice.h"
#include "SpriteComponent.h"
#include "ContactListener.h"
#include "RidgidBodyComponent.h"
#include "Texture.h"

#include "GameObject.h"
#include "Initializers.h"


const float PhysicsDevice::fPRV{ 10.0f };

PhysicsDevice::~PhysicsDevice()
{
}

PhysicsDevice::PhysicsDevice(Vector2D gravity, GraphicsDevice* gDevice) :gravity(gravity.x, gravity.y)
{
	world = std::make_unique<b2World>(GV2PV(gravity));
	ContactListener* c1 = new ContactListener();
	world->SetContactListener(c1);
	B2Debugger = std::make_shared<B2Debug>(gDevice, this);
	B2Debugger->SetFlags(b2Draw::e_shapeBit | b2Draw::e_aabbBit);
	world->SetDebugDraw(B2Debugger.get());
}

//**************************************
//Steps the physics world
bool PhysicsDevice::update(float dt)
//**************************************
{
	world->Step(dt, 8, 3);

	return true;
}

//**************************************
//Moves body to a set location & angle
bool PhysicsDevice::setTransform(const RidgidBody* object, Vector2D position, eFloat angle)
//**************************************
{
	//finds which body this object is attached to.
	//based on value set in UserData field of body
	b2Body* body = FindBody(object);

	//Applies' Box2D transform.
	body->SetTransform
	(
		GV2PV(position),
		RW2PWAngle(angle)
	);
	return true;

}
bool PhysicsDevice::setAngle(const RidgidBody* object, eFloat angle)
{
	b2Body* body = FindBody(object);
	body->SetTransform(body->GetPosition(), RW2PWAngle(angle));
	return true;
}

bool PhysicsDevice::accelerate(const RidgidBody* object, Vector2D forceVec)
{
	setLinearImpulse(object, forceVec, PV2GV(FindBody(object)->GetPosition()));
	return false;
}

bool PhysicsDevice::setActive(const RidgidBody* object, bool active) {
	b2Body* body = FindBody(object);
	body->SetActive(active);
	return active;
}

bool PhysicsDevice::setFilterIndex(const RidgidBody* object, int index)
{
	b2Body* body = FindBody(object);
	b2Fixture* fixtures = body->GetFixtureList();
	b2Filter filter;
	filter.groupIndex = index;
	fixtures->SetFilterData(filter);

	return false;
}

void PhysicsDevice::debugDraw()
{
	world->DrawDebugData();
}

//**************************************
//sets angular velocity
bool PhysicsDevice::setAngularVelocity(const RidgidBody* object, eFloat angularVelocity)
//**************************************
{
	b2Body* body = FindBody(object);
	body->SetAngularVelocity(RW2PW(angularVelocity));
	return true;

}

//**************************************
//sets angular velocity
bool PhysicsDevice::setLinearVelocity(const RidgidBody* object, Vector2D linearVelocity)
//**************************************
{
	b2Body* body = FindBody(object);
	b2Vec2 b2LVelocity;
	b2LVelocity.x = PW2RW(linearVelocity.x);
	b2LVelocity.y = PW2RW(linearVelocity.y);
	body->SetLinearVelocity(b2LVelocity);
	return true;
}

//**************************************
//sets a linear impulse based on passed force vector and center vector
bool PhysicsDevice::setLinearImpulse(const RidgidBody* object, Vector2D forceVec, Vector2D forceCenter)
//**************************************
{
	b2Body* body = FindBody(object);
	body->ApplyLinearImpulse
	(
		GV2PV(forceVec),
		body->GetWorldPoint(GV2PV(forceCenter)),
		true
	);
	return true;
}

//**************************************
//sets torque on body based on passed values
bool PhysicsDevice::setTorque(const RidgidBody* object, eFloat torque)
//**************************************
{
	b2Body* body = FindBody(object);
	body->ApplyTorque(RW2PW(torque), true);
	return true;
}

bool PhysicsDevice::setStatic(const RidgidBody* object)
{
	b2Body* body = FindBody(object);
	body->SetType(b2_staticBody);
	return true;
}

bool PhysicsDevice::setStopPhysics(const RidgidBody* object)
{
	b2Body* body = FindBody(object);
	body->SetActive(false);
	return true;
}

//**************************************
//gets Angular velocity of body
eFloat PhysicsDevice::getAngularVelocity(const RidgidBody* object) const
//**************************************
{
	b2Body* body = FindBody(object);
	return (PW2RWAngle(body->GetAngularVelocity()));
}
Vector2D PhysicsDevice::getLinearVelocity(const RidgidBody* object) const
{
	b2Body* body = FindBody(object);
	return(PV2GV(body->GetLinearVelocity()));
}

bool PhysicsDevice::getLive(const RidgidBody * object) const
{
	return (FindBody(object) != nullptr);
}

//**************************************
//gets bodies' position
Vector2D PhysicsDevice::getPosition(const  RidgidBody* object) const
//**************************************
{
	//b2Body* body = FindBody(object);
	//PV2GV = Physics vector to Game vector (b2vec2 to Vector2D)
	//return (PV2GV(body->GetPosition()));
	return (alignCenters(object));
}

//**************************************
//gets bodies' angle
eFloat PhysicsDevice::getAngle(const RidgidBody* object) const
//**************************************
{
	b2Body* body = FindBody(object);
	if (body == nullptr) return 0.f;
	return (PW2RWAngle(body->GetAngle()));
}

eFloat PhysicsDevice::getArea(const RidgidBody * object) const
{
	b2Body* body = FindBody(object);
	return PW2RW(body->GetMass());
}

Vector2D PhysicsDevice::getVelocity(const RidgidBody* object) const
{
	b2Body* body = FindBody(object);
	return (PV2GV(body->GetLinearVelocity()));
}

//**************************************
//Creates a body and fixture for a passed in object
//based on passed in values
bool PhysicsDevice::createFixture(RidgidBody* object, BodyPresets presets)
//**************************************
{
	//new body definition
	b2BodyDef* bd = new b2BodyDef;
	//may need one or the other, depending on what was passed.
	b2PolygonShape pShape;
	b2CircleShape cShape;

	//Fixture definition
	b2FixtureDef shapefd;
	shapefd.filter.groupIndex = presets.physics.filter;

	//set userData field with passed in object pointer.
	bd->userData = object;


	//set body type
	switch (presets.physics.bodyType)
	{
	case BodyType::Static:
		bd->type = b2_staticBody;
		break;
	case BodyType::Kinematic:
		bd->type = b2_kinematicBody;
		break;
	case BodyType::Dynamic:
		bd->type = b2_dynamicBody;
		break;
	}

	//********Adjust postion because SDL is top left, while box2d is center*************
	presets.position.x += presets.physics.center.x;
	presets.position.y += presets.physics.center.y;
	//**********************************************************************************

	// set starting position & angle
	bd->position.Set(RW2PW(presets.position.x), RW2PW(presets.position.y));
	bd->angle = RW2PWAngle(presets.angle);

	//add the body to the world
	b2Body* body = world->CreateBody(bd);

	//set damping values on the body
	body->SetAngularDamping(presets.physics.angularDamping);
	body->SetLinearDamping(presets.physics.linearDamping);

	//set fixture's shape
	switch (presets.physics.bodyShape)
	{
	case BodyShape::Rectangle:
		//rectangle's dimensions
		pShape.SetAsBox(RW2PW(presets.physics.center.x), RW2PW(presets.physics.center.y));
		shapefd.shape = &pShape;
		break;
	case BodyShape::Circle:
		//circle radius based on object's width.
		cShape.m_radius = RW2PW(presets.physics.radius);
		shapefd.shape = &cShape;
		break;
	case BodyShape::Polygon:
		 //points of the polygon
		b2Vec2 polypoints[8];
		int limit = min((int)presets.physics.poly.size(), 16);
		for (int p = 0; p < limit; p += 2) {
			eFloat a = RW2PW(presets.physics.poly[p]);
			eFloat b = RW2PW(presets.physics.poly[p+1]);
			//printf("\t%f\t%f\n",a,b);
			polypoints[p / 2].Set(a, b);
		}
		pShape.Set(polypoints, presets.physics.poly.size() / 2);
		presets.physics.poly.clear();
		shapefd.shape = &pShape;
		break;
	}

	//set fixture values based on passed in values.
	shapefd.density = presets.physics.density;
	shapefd.friction = presets.physics.friction;
	shapefd.restitution = presets.physics.restitution;

	//create the fixture on the body.
	body->CreateFixture(&shapefd);
	body->SetActive(presets.physics.physicsOn);

	return true;

}

//**************************************
//Removes an object from the World
bool PhysicsDevice::removeObject(RidgidBody* object)
//**************************************
{
	//Find the body based on the passed object
	b2Body* body = FindBody(object);
	destroyJoint(body);

	//check to see if we have a body.
	if (body == NULL)
	{
		return false;
	}
	else
	{
		//delete the body from the world
		world->DestroyBody(body);
		return true;
	}

}

//**************************************
//Destroys a  physics joint from outside the physics device
bool PhysicsDevice::destroyJoint(RidgidBody* object)
//**************************************
{
	b2Body* body = FindBody(object);

	//destroy joints associated with body.
	return destroyJoint(body);
}

//**************************************
//Destroys the joint from within the device
bool PhysicsDevice::destroyJoint(b2Body* body)
//**************************************
{
	bool jointFound = false;
	//destroy joints associated with body.
	for (b2JointEdge* j = body->GetJointList(); j != NULL; j = j->next)
	{
		world->DestroyJoint(j->joint);
		jointFound = true;
	}
	return jointFound;
}

//**************************************
//Uses the user data field in the body and compares the memory location
//to the memory location of the passed object pointer.
//Returns the body that has the same memory location in the user data field
b2Body* PhysicsDevice::FindBody(const RidgidBody* object) const
//**************************************
{
	//loop through the bodies
	for (b2Body* body = world->GetBodyList(); body; body = body->GetNext())
	{
		//when we have a match, return it.
		if (object == body->GetUserData())
		{
			return body;
		}
	}
	//if there was no match, return NULL
	return NULL;
}

//**************************************
//Converts passed Vector2D to b2Vec2
b2Vec2 PhysicsDevice::GV2PV(Vector2D gameVec)const
//**************************************
{
	return b2Vec2(RW2PW(gameVec.x), RW2PW(gameVec.y));
}

//**************************************
//converts passed b2Vec2 to Vector2D
Vector2D PhysicsDevice::PV2GV(b2Vec2 physicsVec)const
//**************************************
{
	Vector2D temp;
	temp.x = PW2RW(physicsVec.x);
	temp.y = PW2RW(physicsVec.y);
	return temp;
}

//**************************************
//Creates a joint using the passed objects and anchor points
bool PhysicsDevice::createJoint(Joints joint)
//**************************************
{
	//find corresponding bodies for objects
	b2Body* bodyA = FindBody(joint.BodyA);
	b2Body* bodyB = FindBody(joint.BodyB);
	if (bodyA == NULL || bodyB == NULL)
	{
		return false;
	}
	//TODO::Flush out other joint types!
	switch (joint.type)
	{
	case JointType::Revolute:
	{
		b2RevoluteJointDef revoluteJointDef;
		revoluteJointDef.bodyA = bodyA;
		revoluteJointDef.bodyB = bodyB;
		revoluteJointDef.collideConnected = joint.CollideConnected;
		revoluteJointDef.localAnchorA.Set(RW2PW(joint.AnchorA.x), RW2PW(joint.AnchorA.y));
		revoluteJointDef.localAnchorB.Set(RW2PW(joint.AnchorB.x), RW2PW(joint.AnchorB.y));
		return world->CreateJoint(&revoluteJointDef);
		break;
	}
	case JointType::Distance:
		break;
	case JointType::Prismatic:
		break;
	case JointType::Wheel:
		break;
	case JointType::Weld:
	{
		b2WeldJointDef weldJointDef;
		weldJointDef.bodyA = bodyA;
		weldJointDef.bodyB = bodyB;
		weldJointDef.collideConnected = joint.CollideConnected;
		weldJointDef.localAnchorA.Set(RW2PW(joint.AnchorA.x), RW2PW(joint.AnchorA.y));
		weldJointDef.localAnchorB.Set(RW2PW(joint.AnchorB.x), RW2PW(joint.AnchorB.y));
		weldJointDef.referenceAngle = RW2PWAngle(joint.referenceAngle);
		return world->CreateJoint(&weldJointDef);
		break;
	}
	case JointType::Pulley:
		break;
	case JointType::Friction:
		break;
	case JointType::Gear:
		break;
	case JointType::Mouse:
		break;
	case JointType::Rope:
	{
		b2RopeJointDef ropeJointDef;
		ropeJointDef.bodyA = bodyA;
		ropeJointDef.bodyB = bodyB;
		ropeJointDef.collideConnected = joint.CollideConnected;
		ropeJointDef.localAnchorA.Set(RW2PW(joint.AnchorA.x), RW2PW(joint.AnchorA.y));
		ropeJointDef.localAnchorB.Set(RW2PW(joint.AnchorB.x), RW2PW(joint.AnchorB.y));
		ropeJointDef.maxLength = RW2PW(joint.JointLimit);
		return world->CreateJoint(&ropeJointDef);
		break;
	}
	default:
		break;
	}
	return false;

}

bool PhysicsDevice::createBoundary(RidgidBody* object, int x, int y, int w, int h)
{

	//new body definition
	b2BodyDef* bd = new b2BodyDef;

	b2PolygonShape pShape;

	//Fixture definition
	b2FixtureDef shapefd;

	//set userData field with passed in object pointer.
	bd->userData = object;

	bd->type = b2_staticBody;

	// set starting position & angle
	bd->position.Set(RW2PW((float)x), RW2PW((float)y));
	bd->angle = 0.f;

	//add the body to the world
	b2Body* body = world->CreateBody(bd);

	//set fixture's shape
	pShape.SetAsBox(RW2PW((float)w), RW2PW((float)h));
	shapefd.shape = &pShape;
	
	shapefd.density = 1.f;
	shapefd.friction = 0.f;
	shapefd.restitution = 1.f;

	//create the fixture on the body.
	body->CreateFixture(&shapefd);
	body->SetActive(true);

	return true;
}

//**************************************
//adjusts postion based on the fact that SDL is top left and Box2d uses the center of an object for position.
Vector2D PhysicsDevice::alignCenters(const RidgidBody* object) const
//**************************************
{
	b2Body* body = FindBody(object);
	if (body == nullptr) return { 0.f, 0.f };
	b2Vec2 bpos = body->GetPosition();
	Vector2D pos{
		PW2RW(bpos.x),
		PW2RW(bpos.y)
	};
	return pos - object->center;
}


#endif // !PHYSICSDEVICE