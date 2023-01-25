#ifndef STAGE_H
#define STAGE_H

#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"
#include "gameMap.h"
#include "Entity.h"
#include "input.h"

enum eStageID {
	INTRO,
	GAME,
	CREDITS
};

class Stage
{
public:
	virtual void start() {};
	virtual void render(float time, Image& framebuffer) {}; //empty body
	virtual void update(double elapsed_time, float time) {}; //empty body

	Color bgcolor = (130, 80, 100);;
};

class IntroStage : public Stage{
public:
	virtual void start();
	virtual void render(float time, Image& framebuffer);
	virtual void update(double elapsed_time, float time);
private:
	Image logo;
	GameMap* map;
	Image* tileset;
	Image minifont;
	Image instruct1;
	Image instruct2_1;
	Image instruct2_2;
	Image instruct3;

	int animDist;
	float startTimeIntro;

};

class GameStage : public Stage {
public:
	virtual void start();
	virtual void render(float time, Image& framebuffer);
	virtual void update(double elapsed_time, float time);

private:
	float startTime;
	float t0 = 1.0f;
	const float enemyVelocity = 60.0f;
	const float enemySpawnTime = 0.5f;
	const float velocity = 80.0f;
	const int animLenghtIdle = 4;
	const int animLenghtRun = 6;
	const int animLenghtMeteor = 2;
	const int topScore = 50000;
	int count;
	bool ended;
	bool endScore;

	sPlayer player;
	GameMap* map;
	Image* tileset;
	Image idle1;
	Image run1;
	Image idle2;
	Image run2;
	Image meteor_pink;
	Image meteor_orange;
	Image meteor_red;
	Image meteor_yellow;

	Image minifont;
	Image idle_rosa;
	Image run_rosa;
	Image idle_orange;
	Image run_orange;
	Image idle_red;
	Image run_red;
	Image idle_yellow;
	Image run_yellow;
	Color bgcolor;

	//allocate memory for meteors
	std::vector<Cube> myEnemiesLeft;
	std::vector<Cube> myEnemiesRight;
};

class CreditsStage : public Stage {
public:
	virtual void start();
	virtual void render(float time, Image& framebuffer);
	virtual void update(double elapsed_time, float time);

private:
	Image minifont;
};
#endif