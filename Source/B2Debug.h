#ifndef BOX2DDEBUG
#define BOX2DDEBUG

#include "Box2D.h"

class GraphicsDevice;
class PhysicsDevice;

class B2Debug : public b2Draw
{
public:
	B2Debug(GraphicsDevice* gDevice, PhysicsDevice* pDevice);
	~B2Debug();
	bool Initialize(GraphicsDevice* gDevice, PhysicsDevice* pDevice);
	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color &color);
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color &color);
	void DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color);
	void DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color);
	void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color);
	void DrawTransform(const b2Transform &xf);

private:
	GraphicsDevice* gDevice;
	PhysicsDevice* pDevice;
	int circleAccuracy;
};

#endif // !BOX2DDEBUG