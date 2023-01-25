#include "collision.h"
#include "world.h"
#include "mesh.h"
#include "BorderManager.h"




Collision::Collision(){

}



void Collision::carCollision(Car* car)
{
	center = World::instance->player.car->model.getTranslation() + Vector3(0, 1, 0);

	Vector3 coll;
	Vector3 collnorm;

	if (car->mesh->testSphereCollision(car->model, center, car->mesh->radius/2, coll, collnorm))
		{
		World::instance->player.car->in_use = false;
		World::instance->player.car = car;

		if (car->type == 3)
		{
			World::instance->wintime -= 5;
		}
		else if (car->type == 4)
		{
			World::instance->wintime += 5;
		}

	}
}

//void Collision::wallCollision(EntityMesh* border, RoadType type, bool side, double* dt)
//{
//	center = World::instance->player.car->model.getTranslation() + Vector3(0, 1, 0);
//
//	Vector3 coll;
//	Vector3 collnorm;
//
//	Matrix44 a_min1;
//	Matrix44 a_min2;
//	Matrix44 a_max1;
//	Matrix44 a_max2;
//	Matrix44 b_min;
//	Matrix44 b_max;
//
//	/*matrix1.translate(World::instance->player.car->mesh->box.center.x, World::instance->player.car->mesh->box.center.y, World::instance->player.car->mesh->box.center.z);
//	matrix1.scale(World::instance->player.car->mesh->box.halfsize.x, World::instance->player.car->mesh->box.halfsize.y, World::instance->player.car->mesh->box.halfsize.z);
//	matrix2.translate(border->mesh->box.center.x, border->mesh->box.center.y, border->mesh->box.center.z);
//	matrix2.scale(border->mesh->box.halfsize.x, border->mesh->box.halfsize.y, border->mesh->box.halfsize.z);*/
//
//	a_min1.translate(World::instance->player.car->mesh->aabb_min.x, World::instance->player.car->mesh->aabb_min.y, World::instance->player.car->mesh->aabb_min.z);
//	a_max1.translate(World::instance->player.car->mesh->aabb_max.x, World::instance->player.car->mesh->aabb_max.y, World::instance->player.car->mesh->aabb_max.z);
//	a_min2.translate(World::instance->player.car->mesh->aabb_max.x, World::instance->player.car->mesh->aabb_min.y, World::instance->player.car->mesh->aabb_min.z);
//	a_max2.translate(World::instance->player.car->mesh->aabb_min.x, World::instance->player.car->mesh->aabb_max.y, World::instance->player.car->mesh->aabb_max.z);
//	b_min.translate(border->mesh->aabb_min.x, border->mesh->aabb_min.y, border->mesh->aabb_min.z);
//	b_max.translate(border->mesh->aabb_max.x, border->mesh->aabb_max.y, border->mesh->aabb_max.z);
//
//	a_min1 = a_min1 * World::instance->player.car->model;
//	a_max1 = a_max1 * World::instance->player.car->model;
//	a_min2 = a_min2 * World::instance->player.car->model;
//	a_max2 = a_max2 * World::instance->player.car->model;
//	b_min = b_min * border->getGlobalMatrix();
//	b_max = b_max * border->getGlobalMatrix();
//
//	//border->mesh->testSphereCollision(border->model, center, World::instance->player.car->mesh->radius, coll, collnorm);
//	if (side == 0)
//	{
//		if (AABBIntersectionright(a_min1, a_max1, b_min, b_max))
//		{
//			//Vector3 push_away = normalize(coll - center) * *dt;
//
//			//Vector3 targetPos = World::instance->player.car->model.getTranslation() - push_away;
//
//			//targetPos.y = World::instance->player.car->model.getTranslation().y;
//
//			//World::instance->player.car->model.translate(targetPos.x, targetPos.y, targetPos.z);
//
//			World::instance->player.car->model.rotate(5.0f * DEG2RAD, Vector3(0.0f, -1.0f, 0.0f));
//
//		}
//	}
//	if (side == 1)
//	{
//		if (AABBIntersectionleft(a_min2, a_max2, b_min, b_max))
//		{
//			World::instance->player.car->model.rotate(5.0f * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
//		}
//	}
//
//}

void Collision::wallCollision(Matrix44 model, RoadType type, int size, bool side, double* dt)
{
		Vector3 coll;
		Vector3 collnorm;
		

		Vector3 dir;
		float dist;

		std::vector<std::tuple<Vector3, Vector3>> rays = borderRays(model, type, size, side);
		
		for (int i=0;i<rays.size();i++)
		{
			dir =  normalize(std::get<1>(rays[i])- std::get<0>(rays[i]));
			dist = std::get<0>(rays[i]).distance(std::get<1>(rays[i]));
			if (World::instance->player.car->mesh->testRayCollision(World::instance->player.car->model, std::get<0>(rays[i]), dir, coll, collnorm, dist)) {
				std::cout << "FUNCIONA" << std::endl;
				//World::instance->player.car->model.rotate(5.0f * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
				//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
				World::instance->player.car->physics.v = 0;
				Vector3 push_away = normalize(coll - World::instance->player.car->model.getTranslation()) * *dt;
				std::cout << push_away.x << std::endl;
				World::instance->player.car->model.translateGlobal(push_away.x, 0, -push_away.z);
			}

		}

	

}


bool Collision::AABBIntersectionright(Matrix44 a_min, Matrix44 a_max, Matrix44 b_min, Matrix44 b_max)
{
	//std::cout << b->aabb_min.z << std::endl;
	if (a_min.getTranslation().x > b_max.getTranslation().x || a_min.getTranslation().z > b_max.getTranslation().z)
	{
		return false;
	}
	if (a_max.getTranslation().x < b_min.getTranslation().x || a_max.getTranslation().z < b_min.getTranslation().z)
	{
		return false;
	}
	return true;
}

bool Collision::AABBIntersectionleft(Matrix44 a_min, Matrix44 a_max, Matrix44 b_min, Matrix44 b_max)
{
	//std::cout << b->aabb_min.z << std::endl;
	if (a_min.getTranslation().x > b_max.getTranslation().x || a_min.getTranslation().z > b_max.getTranslation().z)
	{
		return false;
	}
	if (a_max.getTranslation().x < b_min.getTranslation().x || a_max.getTranslation().z < b_min.getTranslation().z)
	{
		return false;
	}
	return true;
}

std::vector<std::tuple<Vector3, Vector3>> Collision::borderRays(Matrix44 model, RoadType type, int size, bool side)
{
	std::vector<std::tuple<Vector3, Vector3>> rays;
	std::vector<Vector3>* points;


	Matrix44 p1;
	Matrix44 p2;

	Mesh* temp;

	const char* coco;

	switch (type)
	{
	case STRAIGHT:

		if (side) {
			p1.translate(-80, 10, (-69.9 / 2) * size);
			p1 = p1 * model;

			p2.translate(80, 10, (-69.9 / 2) * size);
			p2 = p2 * model;

			rays.push_back(std::tuple<Vector3, Vector3>(p1.getTranslation(), p2.getTranslation()));
		}
		else {

			p1.setTranslation(-80, 10, (+69.9 / 2) * size);
			p1 = p1 * model;

			p2.setTranslation(80, 10, (+69.9 / 2) * size);
			p2 = p2 * model;

			rays.push_back(std::tuple<Vector3, Vector3>(p1.getTranslation(), p2.getTranslation()));


		}
		return rays;
		break;
	case LEFT:

		points = BorderManager::instance->getStage(size,side);

		for (int i = 0; i < points->size() - 1; i++) {
			p1.setTranslation(points->at(i).x, 10, points->at(i).z);
			p2.setTranslation(points->at(i + 1).x, 10, points->at(i + 1).z);


			p1.rotateGlobal(M_PI_2, Vector3(0, 1, 0));
			p2.rotateGlobal(M_PI_2, Vector3(0, 1, 0));

			p1 = p1 * model;
			p2 = p2 * model;

			rays.push_back(std::tuple<Vector3, Vector3>(p1.getTranslation(), p2.getTranslation()));
		}

		return rays;
		break;
	case RIGHT:

		points = BorderManager::instance->getStage(size,side);

		for (int i = 0; i < points->size() - 1; i++) {
			p1.setTranslation(points->at(i).x, 10, points->at(i).z);
			p2.setTranslation(points->at(i + 1).x, 10, points->at(i + 1).z);

			p1 = p1 * model;
			p2 = p2 * model;

			rays.push_back(std::tuple<Vector3, Vector3>(p1.getTranslation(), p2.getTranslation()));
		}
		return rays;
		break;
	default:
		//rays = NULL;
		break;
	}

}

std::tuple<Vector2, Vector2> Collision::endRays(Matrix44 model, RoadType type, int size)
{
	Vector2 p1;
	Vector2 p2;

	p1.set(std::get<1>(borderRays(model, type, size, 0).back()).x, std::get<1>(borderRays(model, type, size, 0).back()).z);
	p2.set(std::get<1>(borderRays(model, type, size, 1).back()).x, std::get<1>(borderRays(model, type, size, 1).back()).z);

	return std::make_tuple(p1, p2);
}