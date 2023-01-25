#pragma once

#include "includes.h"
#include "utils.h"

class Physics
{
public:
	float engineForce;
	float mass = 1500;
	float cDrag = 0.4257;
	float cRR = 12.8;
	float cBraking = 2000.0f;
	float fDrag;
	float fRR;
	float fTotal;
	float a;
	float v;

	int typev;

	bool move;

	Physics();

	Physics(int type);

	float Speed(double* dt);

	float Brake(double* dt);

	void update(double* dt);

};