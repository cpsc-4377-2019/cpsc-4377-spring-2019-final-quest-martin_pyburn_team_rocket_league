#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Component.h"
#include "Definitions.h"
#include "SDL.h"

class GraphicsDevice;
class GameObject;
struct ObjectParams;
class ObjectFactory;
class Particle;
class Texture;

struct ParticleParams {
	string texture;
	eFloat x{ 0.f };
	eFloat y{ 0.f };
	eFloat vx{ 0.f };
	eFloat vy{ 0.f };
	eInt ppf{ 0 };
	eInt lifespan{ 0 };
	eInt parttime{ 0 };
	eInt rx{ 0 };
	eInt ry{ 0 };
	eInt rw{ 0 };
	eInt rh{ 0 };
	eFloat cx{ 0.f };
	eFloat cy{ 0.f };
	eFloat angle{ 0.f };
	eFloat rotation{ 0.f };
	eFloat range{ 0.f };
	eFloat speed{ 0.f };
	RGBA color;
	RGBA endcol;
	eFloat variance{ 0.2f };
	// certain parameters are randomized within (variance <= 1 to 1) x passed value
};

class Emitter : public Component {
public:
	Emitter(std::shared_ptr<GameObject>);
	~Emitter();

	bool initialize(GraphicsDevice*, shared_ptr<ParticleParams>);

	void start();
	std::shared_ptr<GameObject> update();
	void finish();

	void draw();

	void setPosition(Vector2D);
	void setAngle(eFloat);

private:

	std::vector<std::shared_ptr<Particle>> particles;
	
	// particle parameters
	std::shared_ptr<ParticleParams> params;
	eInt lifemin{ 0 };
	eInt liferange{ 0 };
	eFloat speedmin{ 0.f };
	eFloat avmin{ 0.f };

	// texture parameters
	std::shared_ptr<Texture> texture;
	std::shared_ptr<SDL_Rect> rect;
	std::shared_ptr<Vector2D> center;

	eFloat randf(eFloat m, eFloat M);

};
#endif // !PARTICLEEMITTER_H
