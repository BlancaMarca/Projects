#include "endStageClear.h"
#include "world.h"
#include "game.h"
#include "StageManager.h"
#include "input.h"
#include "persistency.h"
#include "PlayStage.h"

endStageClear::endStageClear()
{
	gui = renderGUI(Game::instance->window_width, Game::instance->window_height);
	endOption = RESTART;
}

void endStageClear::init()
{
	endOption = RESTART;
}

void endStageClear::render()
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

	gui.renderGUIMenu(420, 100, 500, 150, Texture::Get("data/level_clear.png"), Game::instance->time, false, false);

	gui.renderGUIMenu(410, 355, 150, 150, Texture::Get("data/end_options.png"), Game::instance->time, false, false);

	if (endOption == RESTART)
	{
		gui.renderGUIMenu(320, 300, 10, 10, Texture::Get("data/dot.png"), Game::instance->time, false, false);
	}
	else if (endOption == NEXT_LEVEL)
	{
		gui.renderGUIMenu(320, 350, 10, 10, Texture::Get("data/dot.png"), Game::instance->time, false, false);
	}
	else if (endOption == EMENU)
	{
		gui.renderGUIMenu(320, 400, 10, 10, Texture::Get("data/dot.png"), Game::instance->time, false, false);
	}

	gui.renderGUIMenu(350, 199, 100, 30, World::instance->timeleft_t, Game::instance->time, false, false);
	World::instance->renderNumber(World::instance->timeleft, 450, 200, 20);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	SDL_GL_SwapWindow(Game::instance->window);
}

void endStageClear::update(double *dt)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN) && int(endOption) < 2) {
		endOption = EndOptions(int(endOption) + 1);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_UP) && int(endOption) > 0) {
		endOption = EndOptions(int(endOption) - 1);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_RETURN)) {
		std::stringstream ss;

		switch (endOption)
		{
		case RESTART:
			ss << "data/Maps/level" << World::instance->current_level + 1 << ".txt";
			std::cout << ss.str().c_str() << std::endl;
			World::instance->loadWorld(ss.str().c_str());

			r_stage = dynamic_cast<PlayStage*>(StageManager::instance->getStage(PLAY));
			r_stage->init();

			Game::instance->current_stage = StageManager::instance->getStage(PLAY);

			break;

		case NEXT_LEVEL:
			if (World::instance->current_level < persistency::instance->current_max)
			{
				ss << "data/Maps/level" << World::instance->current_level + 2 << ".txt";
				std::cout << ss.str().c_str() << std::endl;
				World::instance->loadWorld(ss.str().c_str());

				r_stage = dynamic_cast<PlayStage*>(StageManager::instance->getStage(PLAY));
				r_stage->init();

				World::instance->current_level = World::instance->current_level + 1;

				Game::instance->current_stage = StageManager::instance->getStage(PLAY);
			}
			break;

		case EMENU:
			Game::instance->current_stage = StageManager::instance->getStage(START);
			break;

		default:
			break;
		}
	}
}