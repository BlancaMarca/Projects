#include "prop.h"
#include "shader.h"

prop::prop(PropType type)
{
 shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
texture = Texture::Get("data/Props/props.tga");



this->type = type;

	switch (type)
	{
	case TREE1:
mesh = Mesh::Get("data/Props/tree1.obj");
		break;
	case TREE2:
mesh = Mesh::Get("data/Props/tree2.obj");
		break;
	case ROCK1:
mesh = Mesh::Get("data/Props/rock1.obj");
		break;
	case ROCK2:
mesh = Mesh::Get("data/Props/rock2.obj");
		break;
	case BUSH1:
mesh = Mesh::Get("data/Props/bush1.obj");
		break;
	case BUSH2:
mesh = Mesh::Get("data/Props/bush2.obj");
		break;
	default:
		break;
	}
}
