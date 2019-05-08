#include "FragBehavior.h"
#include "RidgidBodyComponent.h"
#include "SpriteComponent.h"
#include "IntegrityComponent.h"
#include "ObjectListComponent.h"
#include "ObjectFactory.h"
#include <random>
#include <functional>

Frag::Frag(std::shared_ptr<GameObject> owner) : Component( owner )
{
}

Frag::~Frag() { }

void Frag::initialize(ObjectFactory* factory) {
	this->factory = factory;
	shared_ptr<Integrity> newIntegrity = make_shared<Integrity>(getOwner());
	ObjectParams params;

	std::shared_ptr<RidgidBody> parentBody = getOwner()->getComponent<RidgidBody>();
	int area = parentBody->getArea();
	int layer = parentBody->layer;
	bool shell = parentBody->shell;
	params.value = area * layer / 5 + (shell ? 100 : 0);
	newIntegrity->initialize(&params);
	getOwner()->addComponent(dynamic_pointer_cast<Component>(newIntegrity));
}

void Frag::start() { }

std::shared_ptr<GameObject> Frag::update()
{
	if ( !getOwner()->live ) {
		std::shared_ptr<ObjectList> children	= getOwner()->getComponent<ObjectList>();
		std::shared_ptr<RidgidBody> parentBody	= getOwner()->getComponent<RidgidBody>();
		std::shared_ptr<Sprite> parentSprite	= getOwner()->getComponent<Sprite>();
		if (children != nullptr && parentBody != nullptr && parentSprite != nullptr) {

			// random bool - http://www.cplusplus.com/reference/random/
			std::default_random_engine generator;
			std::uniform_int_distribution<int> distribution(0, 1);
			auto coin_toss = std::bind(distribution, generator);

			// position children relative to parent
			for( int c = 0; c < children->list.size(); c++ ){
				GameObject* child = children->list[c].get();
				// Assign Planetoid type
				if(child->type == objectTypes::NA)
					child->type = objectTypes::PLANETOID;

				std::shared_ptr<RidgidBody> childBody = child->getComponent<RidgidBody>();
				std::shared_ptr<Sprite> childSprite = child->getComponent<Sprite>();


				// attach texture path to sprite params
				childSprite->setTexture(&parentSprite->getPath());

				// attach frag behavior
				shared_ptr<Frag> newFrag = make_shared<Frag>(children->list[c]);
				newFrag->initialize(factory);
				child->addComponent(dynamic_pointer_cast<Component>(newFrag));

				// position relative to parent
				Vector2D newp	= childBody->getPosition();
				eFloat newa		= childBody->getAngle();

				Vector2D parp	= parentBody->getPosition();
				eFloat para		= parentBody->getAngle();

				Vector2D newv;

				Vector2D ccenter = childBody->getCentroid();
				Vector2D pcenter = parentBody->getCentroid();

				newa += para * RAD_TO_DEG;

				float x = newp.x;
				float y = newp.y;

				// rotate relative position to match parent angle
				newp.x = x * cosf(para) - y * sinf(para);
				newp.y = y * cosf(para) + x * sinf(para);

				newp += parp + pcenter - ccenter;

				float speed = 50 / childBody->getArea();
				float angle = atan2f(
					(newp.y + ccenter.y) - ( parp.y + pcenter.y),
					(newp.x + ccenter.x) - ( parp.x + pcenter.x )
				);

				newv.x = cosf( angle ) * speed;
				newv.y = sinf( angle ) * speed;

				childBody->setTransform(newp, newa);
				childBody->setVelocity(newv);
				childBody->setRotation(( fabsf( parentBody->getRotation() ) + 150 / childBody->getArea() ) * (coin_toss() ? 1 : -1 ));

				child->initialize();
			}
		}
		else if (children == nullptr) {
			shared_ptr<GameObject> power = factory->getPower();
			if (power != nullptr) {
				shared_ptr<RidgidBody> obod = getOwner()->getComponent<RidgidBody>();
				shared_ptr<RidgidBody> body = power->getComponent<RidgidBody>();
				Vector2D vel = obod->getVelocity();
				body->setTransform(obod->getPosition(), 0.f);
				body->setVelocity({ vel.x / 2, vel.y / 2 });
				return power;
			}
		}
	}
	return nullptr;
}

void Frag::finish()
{
}