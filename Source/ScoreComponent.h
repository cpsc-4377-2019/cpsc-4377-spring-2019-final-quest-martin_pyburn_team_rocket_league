#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include "Definitions.h"
#include "RigidBodyComponent.h"
#include "GameObject.h"
#include "Component.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
class GameObject;
class GameLibrary;


using namespace std;
class Score : public Component
{
public:
	Score(std::shared_ptr<GameObject>);
	~Score();
	void initialize(SDL_Renderer* renderer, shared_ptr<ObjectTemplate> temp);

	void Start();
	void Update();
	void Finish();
protected:
	int score;
	SDL_Color textColor = { 255, 255, 255 };
	SDL_Texture* textSheetTexture = nullptr;
	SDL_Renderer* renderer;
	TTF_Font* normalFont;
}

#endif