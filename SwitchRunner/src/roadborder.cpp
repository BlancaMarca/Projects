#include "roadborder.h"
#include "shader.h"
#include <sstream>

roadborder::roadborder(RoadType type,int s,bool l)
{

    std::stringstream ss;
    ss << "data/Road/";
    switch (type)
    {
    case STRAIGHT:
        ss << "Straight";
        break;
    case LEFT:
        ss << "Curve";
        this->model.rotate(M_PI_2, Vector3(0, 1, 0));
        break;
    case RIGHT:
        ss << "Curve";
        break;
    }

    if (l)
        ss << s << "bl.obj";
    else
        ss << s << "br.obj";

    this->mesh = Mesh::Get(ss.str().c_str());
    this->texture = Texture::Get("data/Road/roadtexture.tga");



    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");//CARGAR LOS SHADER AQUI ES IGUAL LOCURA

}