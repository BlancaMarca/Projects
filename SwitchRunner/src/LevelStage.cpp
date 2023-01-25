#include "LevelStage.h"
#include "game.h"
#include "persistency.h"
#include "input.h"
#include "world.h"
#include "StageManager.h"
#include "PlayStage.h"

LevelStage::LevelStage()
{
this->selected_level = 0;

guiLevel = renderGUI(Game::instance->window_width, Game::instance->window_height);

level1 = 410;
level2 = 1210;
level3 = 2010;
ori = 0;

move_right = false;
move_left = false;
}

void LevelStage::render()
{
	//RENDER TEMPORAL
	//set the clear color (the background color)
	glClearColor(0.565, 0.933, 0.565, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	guiLevel.renderGUIMenu(level1, 300, 450, 250, Texture::Get("data/level1.png"), Game::instance->time, false, false);
	guiLevel.renderGUIMenu(level1, 450, 100, 35, Texture::Get("data/level1_select.png"), Game::instance->time, false, false);

	guiLevel.renderGUIMenu(level2, 300, 450, 250, Texture::Get("data/level2.png"), Game::instance->time, false, false);
	guiLevel.renderGUIMenu(level2, 450, 100, 35, Texture::Get("data/level2_select.png"), Game::instance->time, false, false);

	guiLevel.renderGUIMenu(level3, 300, 450, 250, Texture::Get("data/level3.png"), Game::instance->time, false, false);
	guiLevel.renderGUIMenu(level3, 450, 100, 35, Texture::Get("data/level3_select.png"), Game::instance->time, false, false);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);


	SDL_GL_SwapWindow(Game::instance->window);
}

void LevelStage::update(double* dt)
{
	if (move_right == true)
	{
		level1 -= 800 * *dt;
		level2 = level1 + 800;
		level3 = level2 + 800;
		if (level1 <= ori - 800)
		{
			move_right = false;
		}
	}
	if (move_left == true)
	{
		level1 += 800 * *dt;
		level2 = level1 + 800;
		level3 = level2 + 800;
		if (level1 >= ori + 800)
		{
			move_left = false;
		}
	}

	if (!move_right && !move_left)
	{

		if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT)) {
			if (selected_level < persistency::instance->current_max)
			{
				selected_level++;

				move_right = true;
				ori = level1;
			}
		}

		if (Input::wasKeyPressed(SDL_SCANCODE_LEFT)) {
			if (selected_level > 0)
			{
				selected_level--;

				move_left = true;
				ori = level1;
			}
		}

		if (Input::wasKeyPressed(SDL_SCANCODE_RETURN)) {
			//INICIAR EL NIVEL Y PASAR A LA PLAYSTAGE
			std::stringstream ss;
			ss << "data/Maps/level" << this->selected_level + 1 << ".txt";
			World::instance->loadWorld(ss.str().c_str());

			World::instance->current_level = this->selected_level;

			PlayStage* temp = dynamic_cast<PlayStage*>(StageManager::instance->getStage(PLAY));
			temp->init();

			Game::instance->current_stage = StageManager::instance->getStage(PLAY);
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_BACKSPACE)) {

			Game::instance->current_stage = StageManager::instance->getStage(START);
		}
	}
}

