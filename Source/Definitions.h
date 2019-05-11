#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <cmath>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

typedef unsigned int	Uint32;
typedef float			eFloat;
typedef Uint32			eInt;

struct Vector2D
{
	eFloat x;
	eFloat y;

	friend Vector2D operator+ (const Vector2D & src1, const Vector2D & src2)
	{
		return { src1.x + src2.x, src1.y + src2.y };
	}
	friend Vector2D& operator+=(Vector2D& src1, const Vector2D& src2)
	{
		src1.x += src2.x;
		src1.y += src2.y;
		return src1;
	}
	friend Vector2D operator% (const Vector2D & src1, const Vector2D & src2)
	{
		return {
			fmod(fmod(src1.x, src2.x) + src2.x, src2.x),
			fmod(fmod(src1.y, src2.y) + src2.y, src2.y) };
	}
	friend Vector2D operator- (const Vector2D & src1, const Vector2D & src2)
	{
		return { src1.x - src2.x, src1.y - src2.y };
	}
	friend bool operator> (const Vector2D & src1, int src2)
	{
		if (src1.x > src2 && src1.y > src2)
		{
			return true;
		}
		return false;
	}
	friend bool operator< (const Vector2D & src1, int src2)
	{
		if (src1.x < src2 && src1.y < src2)
		{
			return true;
		}
		return false;
	}
	friend bool operator== (const Vector2D & src1, const Vector2D & src2)
	{
		if (src1.x == src2.x && src1.y == src2.y)
		{
			return true;
		}
		return false;
	}
	friend Vector2D abs(Vector2D & src)
	{
		return { std::abs((float)src.x),std::abs((float)src.y) };
	}
};

struct RGBA
{
	eInt R{ 255 };
	eInt G{ 255 };
	eInt B{ 255 };
	eInt A{ 255 };

	// Apply values with a hexcode 0xRRGGBB
	void setRGB(eInt C) {
		R = C / 0x10000;
		C %= 0x10000;
		G = C / 0x100;
		B = C % 0x100;
	}
	void setAlpha(eInt alpha) {
		A = alpha;
	}
};

//Constants
const eFloat PI = 3.14159f;
const eFloat DEG_TO_RAD = PI / 180.0f;
const eFloat RAD_TO_DEG = 180.0f / PI;
#endif