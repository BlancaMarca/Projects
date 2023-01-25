#include "roadroad.h"
#include "shader.h"
#include "texture.h"
#include <sstream>

roadroad::roadroad(RoadType type,int s)
{
    std::stringstream ss;
    ss << "data/Road/";
	switch (type)
	{
    case STRAIGHT:
        ss << "Straight"<<s<<".obj";
        break;
    case LEFT:
        ss << "Curve" << s << ".obj";
        this->model.rotate(M_PI_2, Vector3(0, 1, 0));
        break;
    case RIGHT:
        ss << "Curve" << s << ".obj";
        break;
	}
    this->mesh = Mesh::Get(ss.str().c_str());
    this->texture= Texture::Get("data/Road/roadtexture.tga");
   


    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");//CARGAR LOS SHADER AQUI ES IGUAL LOCURA

}