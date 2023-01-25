#pragma once
#include "framework.h"


class Stage
{
public:
	Stage();

	virtual void render();
	virtual void update(double* dt);
};

