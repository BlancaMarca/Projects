#pragma once
#include "EntityMesh.h"

enum PropType : uint8 { TREE1, TREE2, ROCK1, ROCK2, BUSH1, BUSH2 };

class prop : public EntityMesh
{
public:



	PropType type;


	prop(PropType type);


};

