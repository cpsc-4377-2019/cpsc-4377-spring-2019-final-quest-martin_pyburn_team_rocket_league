#ifndef INITIALIZERS_H
#define INITIALIZERS_H

#include "Definitions.h"

class RidgidBody;
class Sprite;
class Texture;



struct RotateBody
{
	eFloat torque;
	eFloat maxAngularVelocity;
	eInt radius;
	Vector2D center;
};

enum class BodyShape { Rectangle, Circle, Polygon };
enum class BodyType { Static, Kinematic, Dynamic };
enum class JointType { Revolute, Distance, Prismatic, Wheel, Weld, Pulley, Friction, Gear, Mouse, Rope, None };

struct Joints
{
	JointType type;
	RidgidBody* BodyA;
	RidgidBody* BodyB;
	bool CollideConnected;
	Vector2D AnchorA;
	Vector2D AnchorB;
	eFloat referenceAngle;
	eFloat JointLimit;

};

struct PhysicsStats
{
	eFloat spinSpeed{ 0.0f };
	BodyType bodyType{ BodyType::Dynamic };
	BodyShape bodyShape{ BodyShape::Circle };
	vector<float> poly;
	eFloat radius{ 0.0f };
	Vector2D center{ 0.0f, 0.0f };
	eFloat density{ 1.0f };
	eFloat friction{ 0.5f };
	eFloat restitution{ 1.0f };
	eFloat angularDamping{ 0.0f };
	eFloat linearDamping{ 0.0f };
	eFloat force{ 1.0f };
	Joints joint;
	bool physicsOn{ false };
	int filter{ 0 };
};
struct BodyPresets
{
	Vector2D position{ 0,0 };
	eFloat angle{ 0.0f };
	PhysicsStats physics;
};

#endif // !INITIALIZERS_H

