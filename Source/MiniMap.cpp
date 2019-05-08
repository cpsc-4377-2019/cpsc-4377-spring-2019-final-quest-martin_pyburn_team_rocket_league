#include "MiniMap.h"
#include "GameObject.h"
#include "RidgidBodyComponent.h"
#include "SpriteComponent.h"
#include "GraphicsDevice.h"

MiniMap::MiniMap(GraphicsDevice* gDevice, string path)
{
	this->gDevice = gDevice;
	mini_ship = make_unique<Texture>(gDevice, path + "mm_ship.png");
	mini_object = make_unique<Texture>(gDevice, path + "mm_object.png");
	mini_ship->setRGBA(ship_color);
}

MiniMap::~MiniMap()
{
}

void MiniMap::draw(
	vector<GameObject*> planetoids,
	vector<GameObject*> powerups,
	vector<GameObject*> enemies,
	GameObject* ship)
{
	if (ship == nullptr)return;
	SDL_Rect mask{
		0, 0,
		(int)(mm_x + gDevice->getWidth()* mm_scale),
		(int)(mm_y + gDevice->getHeight()* mm_scale)
	};
	SDL_RenderSetClipRect(gDevice->getRenderer(), &mask);
	for (int it = 0; it < (int)planetoids.size(); it++)
		drawMapItem(planetoids[it], *ship, planetoid_color);
	for (int it = 0; it < (int)powerups.size(); it++)
		drawMapItem(powerups[it], *ship, powerup_color);
	for (int it = 0; it < (int)enemies.size(); it++)
		drawMapItem(enemies[it], *ship, enemy_color);

	shared_ptr<RidgidBody> body = ship->getComponent<RidgidBody>();

	Vector2D position = {
		mm_x + mm_scale * gDevice->getWidth() / 2,
		mm_y + mm_scale * gDevice->getHeight() / 2
	};
	Vector2D bcenter = body->getCentroid();
	Vector2D center{ bcenter.x * mm_scale, bcenter.y * mm_scale };

	mini_ship->setWH((int)(100.f * mm_scale), (int)(83.f * mm_scale));
	mini_ship->draw(
		position,
		body->getAngle(),
		nullptr,
		nullptr
	);
	mask = { 0, 0, gDevice->getWidth(), gDevice->getHeight() };
	SDL_RenderSetClipRect(gDevice->getRenderer(), &mask);
}

void MiniMap::setPosition(float x, float y)
{
	mm_x = x;
	mm_y = y;
}

void MiniMap::drawMapItem(GameObject* obj, GameObject ship, RGBA color) {
	mini_object->setRGBA(color);

	// acquire the required components
	shared_ptr<RidgidBody> shipBody = ship.getComponent<RidgidBody>();
	shared_ptr<RidgidBody> body = obj->getComponent<RidgidBody>();
	shared_ptr<Sprite> sprite = obj->getComponent<Sprite>();

	int width = gDevice->getWidth();
	int height = gDevice->getHeight();
	Vector2D bpos = body->getPosition();
	Vector2D spos = shipBody->getPosition();
	Vector2D bcen = body->getCentroid();

	float x = bpos.x + width / 2 - spos.x;
	float y = bpos.y + height / 2 - spos.y;

	Vector2D position = { (float)fmod(x + width, width), (float)fmod(y + height, height) };
	position.x *= mm_scale;
	position.y *= mm_scale;

	mini_object->setWH((int)(sprite->rect->w * mm_scale), (int)(sprite->rect->h * mm_scale));

	Vector2D center{ bcen.x * mm_scale, bcen.y * mm_scale };

	mini_object->draw(
		position,
		body->getAngle(),
		make_shared<Vector2D>(center),
		nullptr,
		(int)(width * mm_scale),
		(int)(height * mm_scale));
}