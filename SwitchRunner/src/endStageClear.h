#pragma once
#include "Stage.h"
#include "collision.h"
#include "renderGUI.h"
#include "utils.h"
#include "PlayStage.h"

enum EndOptions : uint8
{
	RESTART, NEXT_LEVEL, EMENU
};

class endStageClear :public Stage
{
public:

	EndOptions endOption;
	renderGUI gui;

	PlayStage* r_stage;

	endStageClear();

	void init();

	void render();
	void update(double* dt);
};