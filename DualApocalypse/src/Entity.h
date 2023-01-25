#ifndef ENTITY_H
#define ENTITY_H

#include "includes.h"
#include "utils.h"

enum DIRECTION {
	RIGHT,
	LEFT
};
class Cube
{
public:
	Vector2 pos;
	int color;
	bool in_use = false;
	const int r = rand() % 2;

	Vector2 Min() {
		return Vector2(pos.x, pos.y);
	}
	Vector2 Max() {
		return Vector2(pos.x + 10, pos.y + 18);
	}
	bool AABBIntersection(Cube& a, Cube& b)
	{
		if (a.Min().x > b.Max().x || a.Min().y > b.Max().y)
		{
			return false;
		}
		if (a.Max().x < b.Min().x || a.Max().y < b.Min().y)
		{
			return false;
		}
		return true;
	}
};


struct sPlayer {
	DIRECTION dir;
	Cube cube1;
	Cube cube2;
	bool moving;
};
#endif