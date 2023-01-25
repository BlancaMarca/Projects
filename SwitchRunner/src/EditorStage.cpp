#include "EditorStage.h"
#include "input.h"
#include "world.h"
#include <iostream>
#include <fstream>  
#include "framework.h"
#include "road.h"
#include "game.h"
#include "shader.h"
#include "prop.h"


EditorStage::EditorStage()
{
	dir = 0;
	roadsize = 1;
	//temporal
	current_car = CAR1;
	current_road_i = -1;
	selected_lane = 1;

}

void EditorStage::render()
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	World::instance->camera->enable();


	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	World::instance->render();

	//drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
	drawText(600,2, "Selected:"+Car::enum_string[int(current_car)], Vector3(1, 1, 1), 2);
	drawText(600, 25, "Selected lane:"+ std::to_string(selected_lane), Vector3(1, 1, 1), 2);

	SDL_GL_SwapWindow(Game::instance->window);
}

void EditorStage::update(double* dt)
{
#pragma region Carreteras
	if (Input::wasKeyPressed(SDL_SCANCODE_UP)) {
		Road* temp = new Road(STRAIGHT,roadsize);

		temp->model.rotateGlobal(dir * M_PI_2, Vector3(0, 1, 0));
		temp->model.translateGlobal(pos.x, pos.y, pos.z);
		
		pos.set(pos.x + 80*2*vec[dir].x, 0, pos.z + 80 * 2 * vec[dir].y);

		std::cout << "dir" << std::endl;
		std::cout << dir << std::endl;
		std::cout<<"xyz"<<std::endl;
		std::cout <<pos.x << std::endl;
		std::cout << pos.y << std::endl;
		std::cout << pos.z << std::endl;
		World::instance->roadmap.addChild(temp);
		selectRoad(World::instance->roadmap.children.size()-1);
	};
	if (Input::wasKeyPressed(SDL_SCANCODE_LEFT)) {

		Road* temp = new Road(LEFT, roadsize);

		temp->model.rotateGlobal(dir * M_PI_2, Vector3(0, 1, 0));
		temp->model.translateGlobal(pos.x+ (69.9*(roadsize-1))*vec[dir].z, pos.y, pos.z + (69.9*(roadsize-1))*vec[dir].w);

		World::instance->roadmap.addChild(temp);
		dir = (dir + 3) % 4;
		
		pos.set(pos.x+80 * 2 * vec[dir].x - (69.9 * (roadsize - 1)) * vec[dir].w, 0, pos.z + 80*2 * vec[dir].y + (69.9 * (roadsize - 1))*vec[dir].z);
		std::cout << "xyz" << std::endl;
		std::cout << pos.x << std::endl;
		std::cout << pos.y << std::endl;
		std::cout << pos.z << std::endl;
		selectRoad(World::instance->roadmap.children.size()-1);

	};
	if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT)) {

		Road* temp = new Road(RIGHT, roadsize);

		temp->model.rotateGlobal(dir * M_PI_2, Vector3(0, 1, 0));
		temp->model.translateGlobal(pos.x, pos.y, pos.z);

		World::instance->roadmap.addChild(temp);
		dir = (dir + 1) % 4;

		pos.set(pos.x + 80 * 2 * vec[dir].x + (69.9 * (roadsize - 1)) * vec[dir].z, 0, pos.z + 80 *2 * vec[dir].y + (69.9 * (roadsize - 1)) * vec[dir].w);
		std::cout << "xyz" << std::endl;
		std::cout << pos.x << std::endl;
		std::cout << pos.y << std::endl;
		std::cout << pos.z << std::endl;
		selectRoad(World::instance->roadmap.children.size()-1);
	};
	if (Input::wasKeyPressed(SDL_SCANCODE_P)) {
		this->roadsize = (this->roadsize % 4) + 1;
		std::cout << this->roadsize << std::endl;
	}
	if(Input::wasKeyPressed(SDL_SCANCODE_Z)){
		selectRoad(current_road_i - 1);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_X)) {
		selectRoad(current_road_i + 1);
	}
#pragma endregion

#pragma region Coches
	if (Input::wasKeyPressed(SDL_SCANCODE_TAB)) {
		current_car = CarType(current_car + 1);
		if (current_car == COUNT)
			current_car = CarType(0);
}
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
		World::instance->AddCar(current_car,current_road_i);
		
		float v = ((69.9) / 2) * (current_road->size - 1) - (69.9)* (selected_lane - 1);

		Vector3 p = current_road->getPosition()- Vector3(v*(dir%2),0,v*((dir-1)%2));
		World::instance->pool_cars.back()->model.translateGlobal(p.x,0,p.z);
		World::instance->pool_cars.back()->model.rotate(dir * M_PI_2, Vector3(0, 1, 0));
	}
	if(Input::wasKeyPressed(SDL_SCANCODE_C)){
		if (selected_lane == current_road->size) {
			selected_lane = 1;
		}
		else {
			selected_lane++;
		}
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_V)) {
		if (selected_lane == 1) {
			selected_lane = current_road->size;
		}
		else {
			selected_lane--;
		}
	}
#pragma endregion

	if (Input::wasKeyPressed(SDL_SCANCODE_1)) {

		AddFront(TREE1);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_2)) {

		AddFront(TREE2);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_3)) {

		AddFront(ROCK1);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_4)) {

		AddFront(ROCK2);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_5)) {

		AddFront(BUSH1);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_6)) {

		AddFront(BUSH2);
	}

#pragma region SaveLoad
	if (Input::wasKeyPressed(SDL_SCANCODE_G)) {
		saveMap();
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_L)) {
		std::cout << "Escribe el mapa a cargar:";

		std::string mapname;
		std::cin >> mapname;
		World::instance->loadWorld(mapname.c_str());

	}
#pragma endregion

controlCamera(dt);

}

void EditorStage::controlCamera(double* dt)
{

	float speed = 100 * *dt;

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT)) //is left button pressed?
	{
		World::instance->camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		World::instance->camera->rotate(Input::mouse_delta.y * 0.005f, World::instance->camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}

	//async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) ) World::instance->camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) ) World::instance->camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) ) World::instance->camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) ) World::instance->camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);


}

void EditorStage::saveMap()
{
	std::cout << "Escribe el mapa a guardar:";
	std::string name;
	std::cin >> name;
	std::cout << "Escribe el tiempo para ganar:";
	std::cin >> World::instance->wintime;
	std::ofstream outfile("data/Maps/"+name+".txt");

	outfile << World::instance->wintime << std::endl;

	for (int i = 0; i < World::instance->roadmap.children.size(); i++)
	{
		Road* current = (Road*)(World::instance->roadmap.children[i]);
		outfile << "r" << " " << (int)current->roadtype << " " << current->size;
		
		//Escribimos la model matrix
		for (int j = 0; j < 16; j++)
		{
			outfile << " "<< current->model.m[j];
		}
		outfile<<std::endl;
	}

	for (int i = 0; i < World::instance->pool_cars.size(); i++)
	{
		Car* current = World::instance->pool_cars[i];
		outfile << "c" << " " << (int)current->type<<" "<<current->roadpos;

		//Escribimos la model matrix
		for (int j = 0; j < 16; j++)
		{
			outfile << " " << current->model.m[j];
		}
		outfile << std::endl;

	}

	for (int i = 0; i < World::instance->props.size(); i++)
	{
		prop current = World::instance->props[i];
		outfile << "p" << " " << (int)current.type;

		//Escribimos la model matrix
		for (int j = 0; j < 16; j++)
		{
			outfile << " " << current.model.m[j];
		}
		outfile << std::endl;

	}


	outfile.close();
	std::cout << "Map saved at data/Maps/" + name + ".txt" << std::endl;
}

void EditorStage::selectRoad(int cr)
{
	if (current_road_i >= 0) {

		Entity* temp = current_road->children[2];
		EntityMesh* temp2 = (EntityMesh*)temp;
		temp2->renderbox = FALSE;
	}

	current_road_i = cr;

	current_road = (Road*)World::instance->roadmap.children[current_road_i];

    Entity* temp =current_road->children[2];
	EntityMesh* temp2 = (EntityMesh*)temp;

	temp2->renderbox = TRUE;


}

void EditorStage::AddFront(PropType type)
{
	Vector3 origin = World::instance->camera->eye;
	Vector3 dir = World::instance->camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, Game::instance->window_width, Game::instance->window_height);
	Vector3 pos = RayPlaneCollision(Vector3(), Vector3(0, 1, 0), origin, dir);

	World::instance->props.push_back(prop(type));
	World::instance->props.back().model.setTranslation(pos.x, pos.y, pos.z);
}
