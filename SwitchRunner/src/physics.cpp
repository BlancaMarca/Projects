#include "physics.h"

Physics::Physics()
{
}

Physics::Physics(int type)
{
	a = 0;
	v = 0;
	fDrag = 0;
	fRR = 0;
	fTotal = 0;
	move = true;

	typev = type;

	engineForce = 500;

}

void Physics::update(double* dt)
{
	if (move)
	{
		if ((typev == 0 && v < 2) || typev == 1 && v < 1 || typev == 2 && v < 3 || typev == 3 && v < 2 || typev == 4 && v < 2)
		{
			v = Speed(dt);
		}
	}
	else
	{
		if (v < 0.1f)
		{
			v = 0;
			fTotal = 0;
		}
		else
		{
			v = Brake(dt);
		}
	}
}

float Physics::Speed(double* dt)
{
	fDrag = cDrag * v * v;
	cDrag = cRR * v;

	fTotal = engineForce - fDrag - cDrag;

	a = fTotal / mass;

	return v + *dt * a;
}

float Physics::Brake(double* dt)
{
	fDrag = cDrag * v * v;
	cDrag = cRR * v;

	fTotal = -cBraking - fDrag - cDrag;

	a = fTotal / mass;

	return v + *dt * a;
}