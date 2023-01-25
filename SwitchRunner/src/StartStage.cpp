#include "StartStage.h"
#include "game.h"
#include "input.h"
#include "StageManager.h"
#include "persistency.h"
#include "world.h"
#include "extra/bass.h"

const std::string StartStage::enumm_string[] = { "PLAY_GAME", "EDITOR_START", "EXIT_GAME" };
World* menuWorld = NULL;

StartStage::StartStage()
{
	this->option = PLAY_GAME;

	std::stringstream ss;
	ss << "data/Maps/level" << 1 << ".txt";
	World::instance->loadWorld(ss.str().c_str());

	menuWorld = World::instance;

	gui = renderGUI(Game::instance->window_width, Game::instance->window_height);

	//Inicializamos BASS al arrancar el juego (id_del_device, muestras por segundo, ...)
	if (BASS_Init(-1, 44100, 0, 0, NULL) == false) //-1 significa usar el por defecto del sistema operativo
	{
		//error abriendo la tarjeta de sonido...
	}

	//El handler para un sample
	HSAMPLE hSample;

	//El handler para un canal
	HCHANNEL hSampleChannel;

	//Cargamos un sample del disco duro (memoria, filename, offset, length, max, flags)
	//use BASS_SAMPLE_LOOP in the last param to have a looped sound
	hSample = BASS_SampleLoad(false, "data/Mi_rarro_me_lo_cobaron.mp3", 0, 0, 3, BASS_SAMPLE_LOOP);
	if (hSample == 0)
	{
		//file not found
	}

	//Creamos un canal para el sample
	hSampleChannel = BASS_SampleGetChannel(hSample, false);


	//Lanzamos un sample
	BASS_ChannelPlay(hSampleChannel, true);
}

void StartStage::render(){
	//RENDER TEMPORAL
		//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	menuWorld->camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	menuWorld->render();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gui.renderGUIMenu(410, 200, 500, 250, Texture::Get("data/tit.tga"), Game::instance->time, false, true);
	gui.renderGUIMenu(390, 375, 90, 50, Texture::Get("data/play.png"), Game::instance->time, false, false);
	gui.renderGUIMenu(395, 435, 100, 50, Texture::Get("data/editor.png"), Game::instance->time, false, false);
	gui.renderGUIMenu(390, 495, 100, 40, Texture::Get("data/tutorial.png"), Game::instance->time, false, false);
	gui.renderGUIMenu(390, 555, 75, 50, Texture::Get("data/exit.png"), Game::instance->time, false, false);

	if (option == PLAY_GAME)
	{
		gui.renderGUIMenu(330, 375, 10, 10, Texture::Get("data/dot.png"), Game::instance->time, false, false);
	}
	else if (option == EDITOR_START)
	{
		gui.renderGUIMenu(330, 435, 10, 10, Texture::Get("data/dot.png"), Game::instance->time, false, false);
	}
	else if (option == TUTORIAL)
	{
		gui.renderGUIMenu(330, 495, 10, 10, Texture::Get("data/dot.png"), Game::instance->time, false, false);
	}
	else if (option == EXIT_GAME)
	{
		gui.renderGUIMenu(330, 555, 10, 10, Texture::Get("data/dot.png"), Game::instance->time, false, false);
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	SDL_GL_SwapWindow(Game::instance->window);
}

void StartStage::update(double* dt)
{
	menuWorld->camera->rotate(15.0f * *dt * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));

	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN) && int(this->option) < 3) {
		this->option = StartOptions(int(this->option) + 1);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_UP) && int(this->option) > 0) {
		this->option = StartOptions(int(this->option) - 1);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_RETURN)) {
		switch (option)
		{
		case PLAY_GAME:
			persistency::instance->ReadLevelFile("data/savefile.txt");
			Game::instance->current_stage = StageManager::instance->getStage(LEVELS);
			break;
		case TUTORIAL:
			Game::instance->current_stage = StageManager::instance->getStage(TUT);
			break;
		case EDITOR_START:
			World::instance->resetWorld();
			Game::instance->current_stage = StageManager::instance->getStage(EDITOR);
			break;
		case EXIT_GAME:
			Game::instance->must_exit = true;
			break;
		default:
			break;
		}
	}
}
