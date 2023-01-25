#pragma once

#include "includes.h"
#include "utils.h"
#include "car.h"
#include "collision.h"

/*enum DIRECTION {
	RIGHT,
	LEFT
};*/

struct sPlayer {
	Vector3 pos;
	//DIRECTION dir;
	Car* car;
	Collision collision;
	bool moving;
};