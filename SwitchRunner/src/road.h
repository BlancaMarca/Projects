#pragma once
#include "EntityMesh.h"



enum RoadType : uint8 { STRAIGHT, LEFT, RIGHT };

class Road : public Entity
{
public:
    RoadType roadtype;
    int size;

    Road(RoadType type,int s);

};