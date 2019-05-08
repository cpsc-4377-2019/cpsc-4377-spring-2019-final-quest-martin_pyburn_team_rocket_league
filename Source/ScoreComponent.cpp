#include "ScoreComponent.h"

Score::Score(shared_ptr<GameObject> owner) : Component(owner)
{
	score = 0;

}

Score::~Score() {}

void Score::initialize(SDL_Renderer* renderer, shared_ptr<ObjectTemplate> temp)
{
	this->renderer = renderer;
}



void Score::Update()
{

	//Creates the text then draws it on the renderer
	string scoreText = std::to_string(score);
	textSheetTexture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(normalFont, scoreText.c_str(), textColor));
	int width = 0, height = 0;
	int textX, textY;
	SDL_QueryTexture(textSheetTexture, NULL, NULL, &width, &height);
	textX = (windowHeight - width - 5);
	textY = 5;
	SDL_Rect renderQuad = { textX, textY, width, height };
	//Render to screen
	SDL_RenderCopy(renderer, textSheetTexture, NULL, &renderQuad);
}
void Score::Start()
{

}

void Score::Finish()
{

}