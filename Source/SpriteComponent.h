#ifndef SPRITE_H
#define SPRITE_H
#include<vector>

#include "Component.h"


class GameObject;
class TextureLibrary;
class Texture;
class ObjectTemplate;
struct Vector2D;
struct SDL_Rect;

using namespace std;

class Sprite : public Component
{
public:
	Sprite(std::shared_ptr<GameObject>);
	~Sprite();
	void initialize(TextureLibrary* library, ObjectTemplate* temp);

	void start();
	std::shared_ptr<GameObject> update();
	void finish();

	void draw();

	void setTexture(string* path);
	void setColor(eInt);
	void setAlpha(eInt);

	shared_ptr<Texture> getTexture();

	string getPath();

	// exposed for mini-map
	shared_ptr<SDL_Rect> rect = make_shared<SDL_Rect>();

protected:
	shared_ptr<Vector2D> center = make_shared<Vector2D>();
	shared_ptr<Texture> texture;
	bool ready{ false };

	TextureLibrary* library;
	string path;

	RGBA color;
};


#endif