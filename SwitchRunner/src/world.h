#pragma once
#include "Car.h"
#include "camera.h"
#include "player.h"
#include "prop.h"
#include "renderGUI.h"

const int MAX_CARS = 5;

class World
{
public:
    
    //delete the copy constructor
    //World(const World&) = delete;
    
    static World* instance;

    int cars_init;

    //Camera
    Camera *camera;
    
    //Pool de cars
    //Car pool_cars[MAX_CARS];
    std::vector<Car*>pool_cars;

    std::vector<prop> props;

    //player
    sPlayer player;
    
    //MAPA, carretera+proprs
    Entity roadmap;

    EntityMesh *skyBox;

    EntityMesh* ground;

    renderGUI guiWorld;

    Texture* tex_0;
    Texture* tex_1;
    Texture* tex_2;
    Texture* tex_3;
    Texture* tex_4;
    Texture* tex_5;
    Texture* tex_6;
    Texture* tex_7;
    Texture* tex_8;
    Texture* tex_9;

    Texture* timeleft_t;
    Texture* tutorial_t;

    //ENDZONE
    Vector3 endorigin;
    Vector3 enddir;

    //Level
    int current_level;

    //Tiempo para ganar
    float wintime;
    int timeleft;

    //constructor
    World();
    
    void render();

    void drawSkyGround();

    void loadWorld(const char* path);
    void AddCar(CarType type,int road);
    void setEnd();

    void renderNumber(int n, int x, int y, int s);

    void resetWorld();
};
