#include "Particle.h"

Particle::Particle()
{
}

Particle::~Particle()
{
}

bool Particle::update()
{
	// determine the current offset amount
	eFloat delta = (eFloat)lifespan / (eFloat)initialspan;

	// adjust color
	color.R = endcol.R - (eInt)(delta * colspan[0]);
	color.G = endcol.G - (eInt)(delta * colspan[1]);
	color.B = endcol.B - (eInt)(delta * colspan[2]);
	color.A = endcol.A - (eInt)(delta * colspan[3]);

	velocity += gravity;
	position += velocity;

	angle += rotation;

	lifespan--;
	return lifespan > 0;
}

void Particle::draw()
{
	texture->draw(position, angle, center, rect, 0, 0, color.R, color.G, color.B, color.A);
}
