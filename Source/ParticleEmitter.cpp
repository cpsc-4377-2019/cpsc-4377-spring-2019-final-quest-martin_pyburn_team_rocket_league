#include "ParticleEmitter.h"
#include "Particle.h"
#include "GraphicsDevice.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "ObjectDefinitions.h"
#include "Texture.h"
#include <random>

Emitter::Emitter(std::shared_ptr<GameObject> owner) : Component(owner)
{
}

Emitter::~Emitter()
{
}

bool Emitter::initialize(GraphicsDevice* gDevice, shared_ptr<ParticleParams> params)
{
	this->params = params;

	this->texture = make_shared<Texture>(gDevice, params->texture);

	rect = std::make_shared<SDL_Rect>(SDL_Rect({
		(int)params->rx,
		(int)params->ry,
		(int)params->rw,
		(int)params->rh })
	);
	// center
	center = make_shared<Vector2D>(Vector2D({
		params->cx,
		params->cy
		})
	);

	// calculate ranges for randomized values
	lifemin = (eFloat)params->parttime * params->variance;
	liferange = params->parttime - lifemin;

	speedmin = params->speed * params->variance;

	avmin = params->rotation * params->variance;

	activated = true;

	return false;
}

void Emitter::start()
{
}

std::shared_ptr<GameObject> Emitter::update()
{
	if (!this->activated)return nullptr;
	for (eInt p = 0; p < particles.size(); ) {
		// update particles
		if (particles[p]->update())p++;
		// purge expired particles
		else particles.erase(particles.begin() + p);
	}
	if (params->lifespan != 0) {
		// create ppf new particles
		for (eInt p = 0; p < params->ppf; p++) {
			shared_ptr<Particle> particle = make_shared<Particle>();
			eInt span = rand() % (lifemin + liferange);
			particle->lifespan = span;
			particle->initialspan = span;
			
			particle->rect = rect;
			particle->center = center;

			eFloat speed = randf(speedmin, params->speed);
			eFloat angle = randf(params->angle - params->range, params->angle + params->range);
			
			particle->position = Vector2D{ params->x, params->y };
			particle->velocity = Vector2D{ cosf(angle) * speed, sinf(angle) * speed };

			particle->angle = randf(0.f, 360.f);
			particle->rotation = (rand() % 1 - 1) * randf(avmin, params->rotation);

			particle->texture = texture.get();

			particle->color = params->color;
			particle->endcol = params->endcol;

			particle->colspan[0] = params->endcol.R - params->color.R;
			particle->colspan[1] = params->endcol.G - params->color.G;
			particle->colspan[2] = params->endcol.B - params->color.B;
			particle->colspan[3] = params->endcol.A - params->color.A;

			particles.push_back(particle);
		}
		params->lifespan--;
	}
	else getOwner()->live = false;
	return nullptr;
}

void Emitter::finish()
{
}

void Emitter::draw()
{
	if(activated) for (auto p : particles)p->draw();
}

void Emitter::setPosition(Vector2D pos)
{
	params->x = pos.x;
	params->y = pos.y;
}

void Emitter::setAngle(eFloat angle)
{
	params->angle = angle;
}

eFloat Emitter::randf(eFloat m, eFloat M)
{
	return (rand() / (float)RAND_MAX * (M - m)) + m;
}
