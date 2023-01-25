#pragma once
#include "entity.h"
#include "mesh.h"
#include "rendertotexture.h"

class EntityMesh : public Entity
{
public:
    //Attributes of this class 
    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;
    bool renderbox = FALSE;
    float tiling = 1;

    EntityMesh();

    //methods overwritten 
    void render();
    void update(float dt);
    void setTiling(float t);
};

