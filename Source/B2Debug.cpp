#include "B2Debug.h"
#include "GraphicsDevice.h"
#include "PhysicsDevice.h"

B2Debug::B2Debug(GraphicsDevice* gDevice, PhysicsDevice* pDevice)
{
	Initialize(gDevice, pDevice);
}

B2Debug::~B2Debug()
{
}

bool B2Debug::Initialize(GraphicsDevice* gDevice, PhysicsDevice* pDevice)
{
	if (gDevice == nullptr || pDevice == nullptr)return false;
	this->gDevice = gDevice;
	this->pDevice = pDevice;
	return true;
}

void B2Debug::DrawPoint(const b2Vec2 & p, float32 size, const b2Color & color)
{
}

void B2Debug::DrawPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color)
{
	SDL_SetRenderDrawColor(gDevice->getRenderer(), 0, 0, 255, 255);
	for (int i = 0; i < (vertexCount - 1); i++) {
		SDL_RenderDrawLine(
			gDevice->getRenderer(),
			pDevice->PW2RW(vertices[i].x),
			pDevice->PW2RW(vertices[i].y),
			pDevice->PW2RW(vertices[i + 1].x),
			pDevice->PW2RW(vertices[i + 1].y));
	}
	SDL_RenderDrawLine(
		gDevice->getRenderer(),
		pDevice->PW2RW(vertices[vertexCount - 1].x),
		pDevice->PW2RW(vertices[vertexCount - 1].y),
		pDevice->PW2RW(vertices[0].x),
		pDevice->PW2RW(vertices[0].y));
	SDL_SetRenderDrawColor(gDevice->getRenderer(), 0, 0, 0, 255);
}

void B2Debug::DrawSolidPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color)
{
	float step = 255 / vertexCount;
	for (int i = 0; i < (vertexCount - 1); i++) {
		SDL_SetRenderDrawColor(gDevice->getRenderer(), i * step, 255 - i * step, 0, 255);
		SDL_RenderDrawLine(
			gDevice->getRenderer(),
			pDevice->PW2RW(vertices[i].x),
			pDevice->PW2RW(vertices[i].y),
			pDevice->PW2RW(vertices[i + 1].x),
			pDevice->PW2RW(vertices[i + 1].y));
	}
	SDL_SetRenderDrawColor(gDevice->getRenderer(), 255, 0, 0, 255);
	SDL_RenderDrawLine(
		gDevice->getRenderer(),
		pDevice->PW2RW(vertices[vertexCount - 1].x),
		pDevice->PW2RW(vertices[vertexCount - 1].y),
		pDevice->PW2RW(vertices[0].x),
		pDevice->PW2RW(vertices[0].y));
	SDL_SetRenderDrawColor(gDevice->getRenderer(), 0, 0, 0, 255);
}

void B2Debug::DrawCircle(const b2Vec2 & center, float32 radius, const b2Color & color)
{
}

void B2Debug::DrawSolidCircle(const b2Vec2 & center, float32 radius, const b2Vec2 & axis, const b2Color & color)
{
	eFloat rad = pDevice->PW2RW(radius);
	Vector2D cent;
	cent.x = pDevice->PW2RW(center.x);
	cent.y = pDevice->PW2RW(center.y);
	SDL_SetRenderDrawBlendMode(gDevice->getRenderer(), SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(gDevice->getRenderer(), 64, 255, 64, 128);
	for (int y = -rad; y < rad; y++) {
		int c = cosf(asinf((float)(y / rad))) * rad;

		SDL_RenderDrawLine(
			gDevice->getRenderer(),
			cent.x - c,
			cent.y + y,
			cent.x + c,
			cent.y + y
		);
	}

	SDL_SetRenderDrawColor(gDevice->getRenderer(), 0, 0, 0, 255);
}

void B2Debug::DrawSegment(const b2Vec2 & p1, const b2Vec2 & p2, const b2Color & color)
{
}

void B2Debug::DrawTransform(const b2Transform & xf)
{
}
