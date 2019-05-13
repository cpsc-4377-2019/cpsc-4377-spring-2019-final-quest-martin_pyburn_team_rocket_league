#include "ForceFieldComponent.h"
#include "GameObject.h"
#include "ObjectFactory.h"
#include "RidgidBodyComponent.h"
#include "Initializers.h"

ForceField::ForceField(std::shared_ptr<GameObject> owner) : Component(owner)
{
}

ForceField::~ForceField()
{
}

void ForceField::initialize(shared_ptr<resource_map> resources, ObjectTemplate* temp)
{
	this->factory = resources->factory.get();
	shared_ptr<RidgidBody> obody = getOwner()->getComponent<RidgidBody>();
	Vector2D opos = obody->getPosition();
	ObjectParams* fieldParams = (*temp)[FORCEFIELD].get();
	// construct drone template
	fieldTemplate = make_shared<ObjectTemplate>();
	ObjectParams* fieldBody = (*fieldTemplate)[BODY].get();
	fieldBody->layer = obody->layer;
	fieldBody->cx = fieldBody->cy = fieldBody->radius = 15.f;
	fieldBody->x = opos.x;
	fieldBody->y = opos.y;
	fieldBody->density = 0.f;
	fieldBody->set = true;
	ObjectParams* fieldSprite = (*fieldTemplate)[SPRITE].get();
	fieldSprite->path = "forcefield.png";
	fieldSprite->sx = fieldSprite->sy = 0.f;
	fieldSprite->sw = fieldSprite->sh = 30.f;
	fieldSprite->color = fieldParams->color;
	fieldSprite->colorSet = fieldParams->colorSet;
	fieldSprite->set = true;
	ObjectParams* fieldInt = (*fieldTemplate)[INTEGRITY].get();
	fieldInt->value = fieldParams->value;
	fieldInt->set = true;
}

void ForceField::start()
{
}

std::shared_ptr<GameObject> ForceField::update()
{
	if (!ready && getOwner()->pass_through == nullptr) {
		shared_ptr<GameObject> myField = make_shared<GameObject>();
		myField->origin = getOwner();

		factory->applyTemplate(fieldTemplate, myField);

		myField->type = objectTypes::COMPONENT;

		shared_ptr<RidgidBody> shipBody = getOwner()->getComponent<RidgidBody>();
		shared_ptr<RidgidBody> fieldBody = myField->getComponent<RidgidBody>();

		// attach field with joint
		Joints joint;
		joint.type = JointType::Revolute;
		joint.CollideConnected = false;
		joint.AnchorA = Vector2D{ 0.f, 0.f };
		joint.AnchorB = Vector2D{ 0.f, 0.f };
		shipBody->joinWith(fieldBody.get(), &joint);

		ready = true;
		return myField;
	}
	return nullptr;
}

void ForceField::finish()
{
}
