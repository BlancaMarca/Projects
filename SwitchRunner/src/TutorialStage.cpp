#include "TutorialStage.h"
#include "world.h"
#include "game.h"
#include "input.h"
#include "StageManager.h"

TutorialStage::TutorialStage()
{
}

void TutorialStage::render()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	World::instance->camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	World::instance->render();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	World::instance->guiWorld.renderGUIMenu(Game::instance->window_width/2, Game::instance->window_height/2, Game::instance->window_width, Game::instance->window_height, World::instance->tutorial_t, Game::instance->time, false, false);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	SDL_GL_SwapWindow(Game::instance->window);
}

void TutorialStage::update(double* dt)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
		
		Game::instance->current_stage = StageManager::instance->getStage(START);
	
	}
}