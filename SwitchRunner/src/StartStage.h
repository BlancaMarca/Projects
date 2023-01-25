#pragma once
#include"Stage.h"
#include <string>
#include "renderGUI.h"

enum StartOptions : uint8
{
	PLAY_GAME, EDITOR_START, TUTORIAL, EXIT_GAME
};

class StartStage : public Stage
{
public:
	StartOptions option;
	static const std::string enumm_string[4];
	renderGUI gui;

	StartStage();

	void render();
	void update(double* dt);
};

