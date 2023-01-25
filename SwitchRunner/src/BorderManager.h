#pragma once
#include <map>
#include "framework.h"

class BorderManager
{
private:
	std::map<std::tuple<int,bool>, std::vector<Vector3>> border_cache;

public:
	static BorderManager* instance;

	std::vector<Vector3>* getStage(int size, bool side);

	BorderManager();

	void LoadAll();

	void ParseFile(const char* name, std::vector<Vector3>* temp);
};
