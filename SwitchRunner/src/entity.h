#pragma once
#include <string>
#include "framework.h"


class Entity
{
public:
    Entity(); //constructor
    //virtual ~Entity(); //destructor
    //pointer to parent
    Entity* parent;

    //Pointer to children
    std::vector<Entity*>children;

    //some attributes
    bool in_use = 0;

    std::string name;
    Matrix44 model;

    //methods overwritten by derived classes 
    virtual void render();
    virtual void update(float elapsed_time);

    //Mesh tree methods
    void addChild(Entity* ent);
    void removeChild(Entity* ent);

    //some useful methods...
    Vector3 getPosition();
    Matrix44 getGlobalMatrix();
};

