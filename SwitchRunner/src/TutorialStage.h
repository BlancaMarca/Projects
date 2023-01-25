#pragma once
#include "Stage.h"
#include "utils.h"


class TutorialStage :public Stage
{
public:


	TutorialStage();

	void render();
	void update(double* dt);
};