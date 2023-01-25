#pragma once
#include "Car.h"
class IA
{
public:
	static void moveIA(Car* car, double* dt);
	static float distancePL(Vector2 p0, Vector2 p1, Vector2 p2);
	static bool segmentIntersection(Vector2 p0, Vector2 p1, Vector2 p3, Vector2 p4 );
	static int orientation(Vector2 p, Vector2 q, Vector2 r);
	static bool onSegment(Vector2 p, Vector2 q, Vector2 r);
private:
	IA();
};

