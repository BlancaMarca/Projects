#pragma once
#include "Stage.h"
#include "renderGUI.h"



class LevelStage : public Stage
{
public:
	int selected_level;

	int level1;
	int level2;
	int level3;
	int ori;

	bool move_right;
	bool move_left;

	LevelStage();

	renderGUI guiLevel;

	void render();
	void update(double* dt);

};
