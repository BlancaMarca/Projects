#include "world.h"
#include "game.h"
#include "extra/textparser.h"
#include "road.h"
#include "mesh.h"
#include "shader.h"
#include "renderGUI.h"

World* World::instance = NULL;


World::World(){

	assert(instance == NULL);

	instance = this;

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	//INIT DEL SKYBOX
	skyBox = new EntityMesh();
	skyBox->mesh = Mesh::Get("data/space_cubemap.ASE");
	skyBox->texture = Texture::Get("data/sky.tga");
	skyBox->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	ground = new EntityMesh();
	ground->mesh = new Mesh();
	ground->mesh->createPlane(10000);
	ground->texture = Texture::Get("data/grass2.tga");
	ground->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	ground->model.translate(0, -5, 0);
	ground->setTiling(300.0f);

	guiWorld = renderGUI(Game::instance->window_width, Game::instance->window_height);

	tex_0 = Texture::Get("data/0.png");
	tex_1 = Texture::Get("data/1.png");
	tex_2 = Texture::Get("data/2.png");
	tex_3 = Texture::Get("data/3.png");
	tex_4 = Texture::Get("data/4.png");
	tex_5 = Texture::Get("data/5.png");
	tex_6 = Texture::Get("data/6.png");
	tex_7 = Texture::Get("data/7.png");
	tex_8 = Texture::Get("data/8.png");
	tex_9 = Texture::Get("data/9.png");

	timeleft_t = Texture::Get("data/time_left.png");
	
	tutorial_t = Texture::Get("data/tuto.png");

	//Number of cars at new level(provisional)
	cars_init = 5;
}

void World::render()
{

	drawSkyGround();
	glEnable(GL_DEPTH_TEST);
	//Llamada al render para todos los coches de la pool
	for (int i = 0; i < pool_cars.size(); i++)
	{
		if (pool_cars[i]->in_use) {
			pool_cars[i]->render();
		

		}
	}

	for (int i = 0; i < props.size(); i++) {
		props[i].render();
	}

	roadmap.render();
}

void World::drawSkyGround()
{
	skyBox->model.setTranslation(this->camera->eye.x, this->camera->eye.y, this->camera->eye.z);
	glDisable(GL_DEPTH_TEST);
	skyBox->render();
	ground->render();
}

void World::loadWorld(const char* path)
{
	//First reset everything:
	//CAMBIARA LA SKYBOX??
	//REVISAR COMO SE ELIMINAN LAS COSASS Y SI SE ACUMULA MEMORIA ETC ETC
	resetWorld();


	//Then read the texfile
	TextParser tp;

	tp.create(path);

	this->wintime = tp.getfloat();

	char* w;

	while (w = tp.getword())
	{

		std::string str(w);

		if (str == "R") {
			RoadType t= RoadType(tp.getint());
			int s = tp.getint();
			Road *temp = new Road(t,s);
			
			//Leemos la model matrix


			for (int i = 0; i < 16; i++)
			{
				temp->model.m[i] = tp.getfloat();
			}
			this->roadmap.addChild(temp);
			tp.nextline();
		}
		else if (str == "C") {
			CarType t = CarType(tp.getint());
			int road = tp.getint();
			this->AddCar(t,road);
			Car* temp = pool_cars.back();

			for (int i = 0; i < 16; i++)
			{
				temp->model.m[i] = tp.getfloat();
			}

			tp.nextline();
		}
		else if (str == "P") {
			PropType t = PropType(tp.getint());

			this->props.push_back(prop(t));

			for (int i = 0; i < 16; i++)
			{
				this->props.back().model.m[i] = tp.getfloat();
			}

			tp.nextline();
		}

	}
	player.car = pool_cars[0];

	setEnd();

	std::cout << "Map Loaded" << std::endl;
	
}

void World::AddCar(CarType type,int road){
	Car *temp = new Car(type);
	temp->roadpos = road;
	temp->in_use = 1;
	pool_cars.push_back(temp);
}

void World::setEnd()
{
	Road* temp = dynamic_cast<Road*>(this->roadmap.children.back());
	
	this->endorigin = temp->model.getTranslation();
	this->endorigin.x = this->endorigin.x - ((69.9) / 2) * temp->size;
	this->endorigin.y = 0;
	this->enddir = Vector3(0, 0, 1);

	//std::cout << "x y z" << endorigin.x <<"  "<< endorigin.y << "  " << endorigin.z << std::endl;
	//std::cout << "x y z" << enddir.x << "  " << enddir.y << "  " << enddir.z << std::endl;
}

void World::renderNumber(int n, int x, int y, int s)
{
	if (n >= 10)
	{
		renderNumber(n / 10, x - 20, y, s);
	}

	int digit = n % 10;

	if (digit == 0)
	{
		guiWorld.renderGUIMenu(x, y, s, s, tex_0, Game::instance->time, false, false);
	}
	else if (digit == 1)
	{
		guiWorld.renderGUIMenu(x, y, s, s, tex_1, Game::instance->time, false, false);
	}
	else if (digit == 2)
	{
		guiWorld.renderGUIMenu(x, y, s, s, tex_2, Game::instance->time, false, false);
	}
	else if (digit == 3)
	{
		guiWorld.renderGUIMenu(x, y, s, s, tex_3, Game::instance->time, false, false);
	}
	else if (digit == 4)
	{
		guiWorld.renderGUIMenu(x, y, s, s, tex_4, Game::instance->time, false, false);
	}
	else if (digit == 5)
	{
		guiWorld.renderGUIMenu(x, y, s, s, tex_5, Game::instance->time, false, false);
	}
	else if (digit == 6)
	{
		guiWorld.renderGUIMenu(x, y, s, s, tex_6, Game::instance->time, false, false);
	}
	else if (digit == 7)
	{
		guiWorld.renderGUIMenu(x, y, s, s, tex_7, Game::instance->time, false, false);
	}
	else if (digit == 8)
	{
		guiWorld.renderGUIMenu(x, y, s, s, tex_8, Game::instance->time, false, false);
	}
	else if (digit == 9)
	{
		guiWorld.renderGUIMenu(x, y, s, s, tex_9, Game::instance->time, false, false);
	}
}

void World::resetWorld()
{
	this->pool_cars.clear();
	this->roadmap.children.clear();
	props.clear();
}