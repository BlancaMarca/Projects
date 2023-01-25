#include "stage.h"
#include "game.h"

#include <cmath>


//menu screen
void IntroStage::start() {

	tileset = new Image();
	tileset->loadTGA("data/tileset1.tga");
	map = loadGameMap("data/mymap.map");

	minifont.loadTGA("data/mini-font-white-4x6.tga");
	logo.loadTGA("data/Logo.tga");
	instruct1.loadTGA("data/instruct1.tga");
	instruct2_1.loadTGA("data/instruct2_1.tga");
	instruct2_2.loadTGA("data/instruct2_2.tga");
	instruct3.loadTGA("data/instruct3.tga");

	startTimeIntro = Game::instance->time;
	animDist = 0;
}

void IntroStage::render(float time, Image& framebuffer) {

	int cs = tileset->width / 16;

	for (int x = 0; x < map->width; ++x)
		for (int y = 0; y < map->height; ++y)
		{
			//get cell info
			sCell& cell = map->getCell(x, y);
			if (cell.type == 0) //skip empty
				continue;
			int type = (int)cell.type;
			//compute tile pos in tileset image
			int tilex = (type % 16) * cs; 	//x pos in tileset
			int tiley = floor(type / 16) * cs;	//y pos in tileset
			Area area(tilex, tiley, cs, cs); //tile area
			int screenx = x * cs; //place offset here if you want
			int screeny = y * cs;
			//avoid rendering out of screen stuff
			if (screenx < -cs || screenx > framebuffer.width ||
				screeny < -cs || screeny > framebuffer.height)
				continue;

			//draw region of tileset inside framebuffer
			framebuffer.drawImage(*tileset, screenx, screeny, area);
		}

	framebuffer.drawImage(logo,1,30 - animDist,160,30);
	framebuffer.drawImage(instruct1, 5, 76, 29, 39);
	framebuffer.drawImage(instruct3, 110, 73, 49, 44);

	if (animDist == 0 || animDist == -1)
	{
		framebuffer.drawImage(instruct2_1, 50, 77, 49, 39);
		framebuffer.drawText("Press space to start", 38, 64, minifont,4,6);
	}
	else
	{
		framebuffer.drawImage(instruct2_2, 50, 77, 49, 39);
	}
}

void IntroStage::update(double elapsed_time, float time) {

	if (time - startTimeIntro > 0.3f)
	{
		if (animDist == 0 || animDist == -1)
		{
			animDist = 1;
			startTimeIntro = time;
		}
		else if (animDist == 1)
		{
			animDist = -1;
			startTimeIntro = time;
		}
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) //if key A was pressed
	{
		Game::instance->changeStage(GAME);
	}
}



///game screen
void GameStage::start() {
	myEnemiesLeft = std::vector<Cube>(500);
	myEnemiesRight = std::vector<Cube>(500);

	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
	idle_rosa.loadTGA("data/idle_rosa.tga");
	run_rosa.loadTGA("data/run_rosa.tga");
	idle_orange.loadTGA("data/idle_orange.tga");
	run_orange.loadTGA("data/run_orange.tga");
	idle_red.loadTGA("data/idle_red.tga");
	run_red.loadTGA("data/run_red.tga");
	idle_yellow.loadTGA("data/idle_yellow.tga");
	run_yellow.loadTGA("data/run_yellow.tga");
	meteor_pink.loadTGA("data/meteor_pink.tga");
	meteor_orange.loadTGA("data/meteor_orange.tga");
	meteor_red.loadTGA("data/meteor_red.tga");
	meteor_yellow.loadTGA("data/meteor_yellow.tga");

	player.cube1.pos = Vector2(30, 80);
	player.cube2.pos = Vector2(110, 80);
	player.cube1.color = 0;
	player.cube2.color = 0;
	endScore = false;

	startTime = Game::instance->time;
	count = 0;
	t0 = 0;

	tileset = new Image();
	tileset->loadTGA("data/tileset1.tga");
	map = loadGameMap("data/mymap.map");
}

void GameStage::render(float time, Image& framebuffer) {
	//Create a new Image (or we could create a global one if we want to keep the previous frame)

	//add your code here to fill the framebuffer
	//...

	//some new useful functions
	framebuffer.fill(bgcolor);								//fills the image with one color
	//framebuffer.drawLine( 0, 0, 100,100, Color::RED );		//draws a line
	//framebuffer.drawImage( sprite, 0, 0 );					//draws full image
	//framebuffer.drawImage( sprite, 0, 0, framebuffer.width, framebuffer.height );			//draws a scaled image
	//size in pixels of a cell, we assume every row has 16 cells so the cell size must be image.width / 16
	int cs = tileset->width / 16;

	//for every cell
	for (int x = 0; x < map->width; ++x)
		for (int y = 0; y < map->height; ++y)
		{
			//get cell info
			sCell& cell = map->getCell(x, y);
			if (cell.type == 0) //skip empty
				continue;
			int type = (int)cell.type;
			//compute tile pos in tileset image
			int tilex = (type % 16) * cs; 	//x pos in tileset
			int tiley = floor(type / 16) * cs;	//y pos in tileset
			Area area(tilex, tiley, cs, cs); //tile area
			int screenx = x * cs; //place offset here if you want
			int screeny = y * cs;
			//avoid rendering out of screen stuff
			if (screenx < -cs || screenx > framebuffer.width ||
				screeny < -cs || screeny > framebuffer.height)
				continue;

			//draw region of tileset inside framebuffer
			framebuffer.drawImage(*tileset, screenx, screeny, area);
		}

	//current animation moving or idle
	int currentAnimMeteor = (int((time - startTime) * 5.0f) % animLenghtMeteor);
	int currentAnimIdle = !player.moving ? (int((time - startTime) * 5.0f) % animLenghtIdle) : 0;
	int currentAnimRun = player.moving ? (int((time - startTime) * 5.0f) % animLenghtRun) : 0;

	//check color to render
	if (player.cube1.color == 0)
	{
		run1 = run_rosa;
		idle1 = idle_rosa;
	}
	else
	{
		run1 = run_orange;
		idle1 = idle_orange;
	}
	if (player.cube2.color == 0)
	{
		run2 = run_red;
		idle2 = idle_red;
	}
	else
	{
		run2 = run_yellow;
		idle2 = idle_yellow;
	}

	//draw meteors
	for (size_t i = 0; i < myEnemiesLeft.size(); i++)
	{
		if (myEnemiesLeft[i].in_use)
		{
			if (myEnemiesLeft[i].r < 1)
			{
				framebuffer.drawImage(meteor_pink, myEnemiesLeft[i].pos.x, myEnemiesLeft[i].pos.y, Area(11 * currentAnimMeteor, 0, 11, 19));
			}
			else
			{
				framebuffer.drawImage(meteor_orange, myEnemiesLeft[i].pos.x, myEnemiesLeft[i].pos.y, Area(11 * currentAnimMeteor, 0, 11, 19));
			}
			if (myEnemiesRight[i].r < 1)
			{
				framebuffer.drawImage(meteor_red, myEnemiesRight[i].pos.x, myEnemiesLeft[i].pos.y, Area(11 * currentAnimMeteor, 0, 11, 19));
			}
			else
			{
				framebuffer.drawImage(meteor_yellow, myEnemiesRight[i].pos.x, myEnemiesLeft[i].pos.y, Area(11 * currentAnimMeteor, 0, 11, 19));
			}
		}
	}

	//draw player
	if (player.moving)//run
	{
		framebuffer.drawImage(run1, player.cube1.pos.x, player.cube1.pos.y, Area(16 * currentAnimRun, 19 * (int)player.dir, 16, 19));	//draws only a part of an image
		framebuffer.drawImage(run2, player.cube2.pos.x, player.cube2.pos.y, Area(16 * currentAnimRun, 19 * (int)player.dir, 16, 19));
	}
	else//idle
	{
		framebuffer.drawImage(idle1, player.cube1.pos.x, player.cube1.pos.y, Area(16 * currentAnimIdle, 19 * (int)player.dir, 16, 19));	//draws only a part of an image
		framebuffer.drawImage(idle2, player.cube2.pos.x, player.cube2.pos.y, Area(16 * currentAnimIdle, 19 * (int)player.dir, 16, 19));
	}
	
	if (endScore == true)
	{
		framebuffer.drawText(toString(topScore), 72, 5, minifont, 4, 6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)

		if (int((time - startTime) * 1000) - topScore > 950)
		{
			framebuffer.drawText("DINOS SURVIVED", 52, 60, minifont, 4, 6);
		}
	}
	else
	{
		framebuffer.drawText(toString(int((time - startTime) * 1000)), 72, 5, minifont, 4, 6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)
	}

}

void GameStage::update(double elapsed_time, float time) {
	//Add here your update method
	//...
	//
	if (int((time - startTime) * 1000) > topScore)
	{
		endScore = true;
		ended = true;

		for (size_t i = 0; i < myEnemiesLeft.size() - 1; i++)
		{
			if (myEnemiesLeft[i].in_use)
			{
				ended = false;
			}
		}

		if (ended == true)
		{
			Game::instance->changeStage(CREDITS);
		}
	}
	else if (time - startTime - t0 > enemySpawnTime)
	{
		myEnemiesLeft[count].in_use = true;
		myEnemiesRight[count].in_use = true;
		count++;
		t0 += enemySpawnTime;
	}

	for (size_t i = 0; i < myEnemiesLeft.size() - 1; i++)
	{
		if (myEnemiesLeft[i].in_use)
		{
			if (myEnemiesLeft[i].pos.y == 0)
			{
				myEnemiesLeft[i].pos.x = rand() % 64;
				myEnemiesRight[i].pos.x = 80 + rand() % 69;
			}
			myEnemiesLeft[i].pos.y += velocity * elapsed_time;
			myEnemiesRight[i].pos.y += velocity * elapsed_time;

			if (player.cube1.AABBIntersection(player.cube1, myEnemiesLeft[i]) && (player.cube1.color != myEnemiesLeft[i].r))
			{
				Game::instance->changeStage(CREDITS);
			}
			if (player.cube2.AABBIntersection(player.cube2, myEnemiesRight[i]) && (player.cube2.color != myEnemiesRight[i].r))
			{
				Game::instance->changeStage(CREDITS);
			}
			if (myEnemiesLeft[i].pos.y > 120)
			{
				myEnemiesLeft[i].in_use = false;
				myEnemiesRight[i].in_use = false;
			}
		}
	}

	player.moving = false;
	//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		if (player.cube1.pos.y > 50)
		{
			player.cube1.pos.y -= velocity * elapsed_time;
			player.cube2.pos.y -= velocity * elapsed_time;
			player.moving = true;
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		if (player.cube1.pos.y < 120 - 19)
		{
			player.cube1.pos.y += enemyVelocity * elapsed_time;
			player.cube2.pos.y += enemyVelocity * elapsed_time;
			player.moving = true;
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key up
	{
		if (player.cube1.pos.x > 0)
		{
			player.cube1.pos.x -= velocity * elapsed_time;
			player.cube2.pos.x -= velocity * elapsed_time;
			player.dir = DIRECTION::LEFT;
			player.moving = true;
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key down
	{
		if (player.cube2.pos.x < 144)
		{
			player.cube1.pos.x += velocity * elapsed_time;
			player.cube2.pos.x += velocity * elapsed_time;
			player.dir = DIRECTION::RIGHT;
			player.moving = true;
		}
	}

	//example of 'was pressed'
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key A was pressed
	{
		if (player.cube1.color == 0)
		{
			player.cube1.color = 1;
		}
		else
		{
			player.cube1.color = 0;
		}
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_X)) //if key Z was pressed
	{
		if (player.cube2.color == 0)
		{
			player.cube2.color = 1;
		}
		else
		{
			player.cube2.color = 0;
		}
	}

	//to read the gamepad state
	if (Input::gamepads[0].isButtonPressed(A_BUTTON)) //if the A button is pressed
	{
	}

	if (Input::gamepads[0].direction & PAD_UP) //left stick pointing up
	{
		bgcolor.set(0, 255, 0);
	}
}

//Death screen
void CreditsStage::start() {
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
}

void CreditsStage::render(float time, Image& framebuffer) {

	framebuffer.drawImage(Game::instance->temp_framebuffer,0,0,160,120);
	framebuffer.drawText( "Press space to restart", 38, 80, minifont, 4, 6);
	framebuffer.drawText("Press M to go to menu", 40, 90, minifont, 4, 6);
}

void CreditsStage::update(double elapsed_time, float time) {
	if (Input::wasKeyPressed(SDL_SCANCODE_M)) //if key A was pressed
	{
		Game::instance->changeStage(INTRO);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) //if key A was pressed
	{
		Game::instance->changeStage(GAME);
	}
}