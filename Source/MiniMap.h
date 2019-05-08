#ifndef MINMAP_H
#define MINMAP_H

#include "Definitions.h"
#include "Texture.h"

class GraphicsDevice;
class GameObject;
class Texture;

class MiniMap {

public:
	MiniMap(GraphicsDevice*, string);
	~MiniMap();
	void draw(vector<GameObject*>, vector<GameObject*>, vector<GameObject*>, GameObject*);
	void setPosition(float = 0.f, float = 0.f);

private:
	GraphicsDevice* gDevice;
	unique_ptr<Texture> mini_ship;
	unique_ptr<Texture> mini_object;
	float mm_scale{ .12f };
	float mm_x{ 0 };
	float mm_y{ 0 };

	void drawMapItem(GameObject*, GameObject, RGBA);

	RGBA ship_color{ 64, 64, 255, 255 };
	RGBA planetoid_color{ 255, 255, 255, 255 };
	RGBA enemy_color{ 255, 128, 0, 255 };
	RGBA powerup_color{ 0, 255, 0, 255 };
};

#endif // MINIMAP_H
