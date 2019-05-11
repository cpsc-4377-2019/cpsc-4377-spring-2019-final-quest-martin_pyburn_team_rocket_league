#ifndef PARTICLE_H
#define PARTICLE_H
#include<vector>

#include "Definitions.h"
#include "Texture.h"


using namespace std;

class Particle
{
public:
	Particle();
	~Particle();

	bool update();

	void draw();

	Texture* texture;

	int initialspan{ 0 };
	int lifespan{ 0 };

	Vector2D position;
	Vector2D velocity;
	eFloat angle;
	eFloat rotation;

	std::shared_ptr<SDL_Rect> rect;
	std::shared_ptr<Vector2D> center;
	
	RGBA color;
	RGBA endcol;
	eInt colspan[4];

};


#endif PARTICLE_H