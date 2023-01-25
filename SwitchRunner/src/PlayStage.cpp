#include "PlayStage.h"
#include "world.h"
#include "camera.h"
#include "utils.h"
#include "game.h"
#include "input.h"
#include "shader.h"
#include "framework.h"
#include "road.h"
#include "StageManager.h"
#include "IA.h"
#include "endStageClear.h"
#include "endStageFail.h"
#include "persistency.h"

World* world = NULL;
Shader* shader = NULL;

PlayStage::PlayStage()
{
	//Asigna los singletons
	world = World::instance;
}

void PlayStage::init()
{
	tex1 = Texture::Get("data/paused.png");
	tex2 = Texture::Get("data/paused_options.png");

	pause = 0;

	guiPause = renderGUI(Game::instance->window_width, Game::instance->window_height);

	option = RESUME;

	camPos = world->player.car->model;

	world->player.car->physics.engineForce = 0;
	world->player.car->physics.v = 0;

	this->start = TRUE;
	this->finish = FALSE;
	elapsed = 0;
	last = 0;

	b = true;
}

void PlayStage::render()
{

	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	world->camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	world->render();

	//Player Camera
	world->camera->eye = camPos * Vector3(-250.0f, 200.0f, 0.0f);
	world->camera->center = camPos * Vector3(0.0f, 0.0f, 0.0f);

	if (start == TRUE)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		World::instance->renderNumber(3 - int(floor(elapsed)), 400, 100, 50);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
	}

	if (start == FALSE && finish == FALSE)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		guiPause.renderGUIMenu(350, 49, 100, 30, World::instance->timeleft_t, Game::instance->time, false, false);
		World::instance->renderNumber(World::instance->wintime - elapsed + 1, 450, 50, 20);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
	}

	if (pause == true)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		guiPause.renderGUIMenu(400, 150, 300, 150, Texture::Get("data/paused.png"), Game::instance->time, false, false);
		guiPause.renderGUIMenu(400, 350, 150, 200, Texture::Get("data/paused_options.png"), Game::instance->time, false, false);

		if (option == RESUME)
		{
			guiPause.renderGUIMenu(300, 275, 10, 10, Texture::Get("data/dot.png"), Game::instance->time, false, false);
		}
		else if (option == RETRY)
		{
			guiPause.renderGUIMenu(300, 345, 10, 10, Texture::Get("data/dot.png"), Game::instance->time, false, false);
		}
		else if (option == MENU)
		{
			guiPause.renderGUIMenu(300, 415, 10, 10, Texture::Get("data/dot.png"), Game::instance->time, false, false);
		}

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
	}

	SDL_GL_SwapWindow(Game::instance->window);
}

void PlayStage::update(double* dt)
{
	if (b)
	{
		*dt = 0;
		b = false;
	}

	if (finish == FALSE && World::instance->wintime - elapsed < 0)
	{
		endf(dt);
		return;
	}

	if (pause == true)
	{
		menuController();
	}
	if (pause == false)
	{
		if (Input::wasKeyPressed(SDL_SCANCODE_SPACE))
		{
			pause = true;
		}

		elapsed = elapsed + *dt;

		if (start == TRUE) {
			if (elapsed < 3) {
				return;
			}
			else {
				start = FALSE;
				elapsed = 0;
			}
		}

		if (start == FALSE && finish == FALSE)
		{
			world->timeleft = int(World::instance->wintime - elapsed + 1);
		}

		if (finish == TRUE) {
			if (world->player.car->physics.move)
			{
				world->player.car->physics.move = false;
			}
			if (elapsed - endtime < 2) {
				physicsUpdate(dt);
				//carWallCollision(dt);
				return;
			}
			else {
				endc(dt);
				return;
			}

		}
		//endCheck(dt);

		playerController(dt);

		//IA::moveIA(world->player.car,dt);

		//COLISION COCHE CON MUROS
		carWallCollision(dt);

		//ESTO K ES, aqui llama para q actualizen las físicas
		physicsUpdate(dt);

		if (world->player.car->roadpos == world->roadmap.children.size()-10) {
			finish = TRUE;
			endtime = elapsed;
			return;
		}

		cameraUpdate(dt);
	}
}

void PlayStage::updateRoadPos(int i)
{
	Car* tempCar = world->pool_cars[i];

	if (tempCar->roadpos < World::instance->roadmap.children.size())
	{

		Road* temp = dynamic_cast<Road*>(World::instance->roadmap.children[tempCar->roadpos]);
		std::tuple<Vector2, Vector2> ray = Collision::endRays(temp->getGlobalMatrix(), temp->roadtype, temp->size);

		Vector2 tempc1;
		Vector2 tempc2;

		tempc1.set(tempCar->model.getTranslation().x, tempCar->model.getTranslation().z);
		tempc2.set(tempc1.x + 5, tempc1.y);

		if (IA::segmentIntersection(std::get<0>(ray), std::get<1>(ray), tempc1, tempc2))
		{
			tempCar->roadpos++;
		}
	}
}

void PlayStage::menuController()
{
	guiPause = renderGUI(Game::instance->window_width, Game::instance->window_height);
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN) && int(option) < 2) {
		option = PauseOptions(int(option) + 1);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_UP) && int(this->option) > 0) {
		option = PauseOptions(int(option) - 1);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_RETURN)) {
		std::stringstream ss;
		switch (option)
		{
		case RESUME:
			pause = false;
			break;
		case RETRY:
			ss << "data/Maps/level" << world->current_level + 1 << ".txt";
			std::cout << ss.str().c_str() << std::endl;
			world->loadWorld(ss.str().c_str());

			init();

			break;
		case MENU:
			Game::instance->current_stage = StageManager::instance->getStage(START);
			break;
		default:
			break;
		}
	}
}

void PlayStage::playerController(double* dt)
{
	if (!Input::isKeyPressed(SDL_SCANCODE_UP) && world->player.car->physics.move)
	{
		if (world->player.car->physics.engineForce > 50)
		{
			world->player.car->physics.engineForce -= 50;
		}
		else
		{
			world->player.car->physics.engineForce = 0;
		}
	}

	if (Input::isKeyPressed(SDL_SCANCODE_UP))
	{
		world->player.car->physics.move = true;

		if (world->player.car->physics.engineForce < 500.0f)
		{
			world->player.car->physics.engineForce += 200.0f;
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN))
	{
		world->player.car->physics.move = false;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_LEFT))
	{
		if (world->player.car->physics.v > 0)
		{
			world->player.car->model.rotate(90.0f * *dt * DEG2RAD, Vector3(0.0f, -1.0f, 0.0f));
			//camPos.rotate(90.0f * *dt * DEG2RAD, Vector3(0.0f, -1.0f, 0.0f));
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT))
	{
		if (world->player.car->physics.v > 0)
		{
			world->player.car->model.rotate(90.0f * *dt * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
			//camPos.rotate(90.0f * *dt * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
		}
	}

	for (size_t i = 0; i < world->pool_cars.size(); i++)
	{
		if (world->pool_cars[i] != world->player.car && world->pool_cars[i]->in_use)
		{
			if (abs(world->player.car->roadpos - world->pool_cars[i]->roadpos) <= 1) {
				world->player.collision.carCollision(world->pool_cars[i]);
			}
			//IA::moveIA(world->pool_cars[i], dt);
		}
	}
}

void PlayStage::endCheck(double* dt)
{
	elapsed = elapsed + *dt;


	if (start == TRUE) {
		if (elapsed < 3) {
			return;
		}
		else {
			start = FALSE;
			elapsed = 0;
		}
	}

	if (finish == TRUE) {
		if (elapsed - endtime < 3) {
			return;
		}
		else {
			endc(dt);
			return;
		}

	}
}

void PlayStage::endc(double* dt)
{
	endStageClear* temp = dynamic_cast<endStageClear*>(StageManager::instance->getStage(ENDC));
	temp->init();

	Game::instance->current_stage = StageManager::instance->getStage(ENDC);
}

void PlayStage::endf(double* dt)
{
	endStageFail* temp = dynamic_cast<endStageFail*>(StageManager::instance->getStage(ENDF));
	temp->init();

	Game::instance->current_stage = StageManager::instance->getStage(ENDF);
}

void PlayStage::physicsUpdate(double* dt)
{
	for (size_t i = 0; i < world->pool_cars.size(); i++)
	{
		updateRoadPos(i);

		if (world->pool_cars[i] == world->player.car)
		{
			world->pool_cars[i]->model.translate(1.0f * 1.5 * world->pool_cars[i]->physics.v, 0.0f, 0.0f);
		}
		else
		{
			world->pool_cars[i]->model.translate(1.0f * world->pool_cars[i]->physics.v, 0.0f, 0.0f);
		}
		world->pool_cars[i]->physics.update(dt);

	}
}

void PlayStage::cameraUpdate(double* dt)
{
	int t = world->player.car->model.m[12];
	int t2 = camPos.m[12];

	int tt = world->player.car->model.m[14];
	int tt2 = camPos.m[14];

	if (t2 <= t)
	{
		camPos.translate(1.0f * 1.5 * world->player.car->physics.v, 0.0f, 0.0f);
	}
	if (tt2 - tt > 50) {
		camPos.translate(0.0f, 0.0f, 50 - (tt2 - tt));
	}
	if (tt2 - tt < -50) {
		camPos.translate(0.0f, 0.0f, -50 - (tt2 - tt));
	}
}

void PlayStage::carWallCollision(double* dt)
{
	for (int i = 0; i < 2; i++)
	{
		Road* temp = dynamic_cast<Road*>(world->roadmap.children[world->player.car->roadpos + i]);
		//world->player.collision.wallcollision(dynamic_cast<entitymesh*>(temp->children[0]), temp->roadtype, false,dt);
		//world->player.collision.wallcollision(dynamic_cast<entitymesh*>(temp->children[1]), temp->roadtype, true, dt);

		world->player.collision.wallCollision(temp->getGlobalMatrix(), temp->roadtype, temp->size, 0, dt);
		world->player.collision.wallCollision(temp->getGlobalMatrix(), temp->roadtype, temp->size, 1, dt);

	}
}