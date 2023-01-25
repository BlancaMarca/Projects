#include "entity.h"

Entity::Entity()
{
	parent = NULL;
}

void Entity::render()
{

	for (int i = 0; i < children.size(); i++)
		children[i]->render();  //repeat for every child
}

void Entity::update(float elapsed_time)
{
}

void Entity::addChild(Entity* ent)
{
	this->children.push_back(ent);
	ent->parent = this;
}

void Entity::removeChild(Entity* ent)
{
	//IMPLEMENTAR REMOVE
	//this->children.
}

Vector3 Entity::getPosition()
{

	return Vector3(this->model._41,this->model._42,this->model._43);
}

Matrix44 Entity::getGlobalMatrix()
{

	if (parent) //if I have a parent, ask his global and concatenate
		return model * parent->getGlobalMatrix();
	return model; //otherwise just return my model as global
}
