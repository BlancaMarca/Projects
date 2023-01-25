#pragma once
#include "Stage.h"
#include "collision.h"
#include "renderGUI.h"
#include "utils.h"
#include "PlayStage.h"

enum EndOptionsF : uint8
{
	RESTARTF, EMENUF
};

class endStageFail :public Stage
{
public:

	EndOptionsF endOptionF;
	renderGUI gui;

	PlayStage* r_stageF;

	endStageFail();

	void init();

	void render();
	void update(double* dt);
};