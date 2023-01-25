#include "BorderManager.h"
#include <cassert>
#include "extra/textparser.h"
#include <string>
#include <sstream>

BorderManager* BorderManager::instance = NULL;


BorderManager::BorderManager()
{
	assert(instance == NULL);

	instance = this;

	this->LoadAll();
}



void BorderManager::ParseFile(const char* name,  std::vector<Vector3>* temp)
{
	TextParser tp;
	tp.create(name);

	char* w;
	

	while (w = tp.getword()) {
		std::string str(w);
		if (str == "V") {
			temp->push_back(Vector3());

			temp->back().x = tp.getfloat();
			temp->back().y = tp.getfloat();
			temp->back().z = tp.getfloat();
		}

	}

	//tp.destroy();

}

void BorderManager::LoadAll()
{
	std::vector<Vector3>* temp;
	temp = new std::vector<Vector3>;

	ParseFile("data/Road/curve1blv.txt", temp);
	std::tuple<int, bool> p;
	p = std::make_tuple(1, true);
	border_cache[p] = *temp;

	std::vector<Vector3>* temp2;
	temp2 = new std::vector<Vector3>;
	ParseFile("data/Road/curve1brv.txt", temp2);
	p = std::make_tuple(1, false);
	border_cache[p] = *temp2;

	std::vector<Vector3>* temp3;
	temp3 = new std::vector<Vector3>;
	ParseFile("data/Road/curve2blv.txt", temp3);
	p = std::make_tuple(2, true);
	border_cache[p] = *temp3;

	std::vector<Vector3>* temp4;
	temp4 = new std::vector<Vector3>;
	ParseFile("data/Road/curve2brv.txt", temp4);
	p = std::make_tuple(2, false);
	border_cache[p] = *temp4;

	std::vector<Vector3>* temp5;
	temp5 = new std::vector<Vector3>;
	ParseFile("data/Road/curve3blv.txt", temp5);
	p = std::make_tuple(3, true);
	border_cache[p] = *temp5;

	std::vector<Vector3>* temp6;
	temp6 = new std::vector<Vector3>;
	ParseFile("data/Road/curve3brv.txt", temp6);
	p = std::make_tuple(3, false);
	border_cache[p] = *temp6;

	std::vector<Vector3>* temp7;
	temp7 = new std::vector<Vector3>;
	ParseFile("data/Road/curve4blv.txt", temp7);
	p = std::make_tuple(4, true);
	border_cache[p] = *temp7;

	std::vector<Vector3>* temp8;
	temp8 = new std::vector<Vector3>;
	ParseFile("data/Road/curve4brv.txt", temp8);
	p = std::make_tuple(4, false);
	border_cache[p] = *temp8;

}

std::vector<Vector3>* BorderManager::getStage(int size, bool side)
{
	std::tuple<int, bool> p;
	p = std::make_tuple(size, side);


	//search stage in cache
	auto it = border_cache.find(p);

	//if found, return it
	if (it != border_cache.end()) {
		return &(it->second);
	}

	//not found, then throw error
	//std::cout << "Error, border not found" << std::endl;

	return nullptr;
}