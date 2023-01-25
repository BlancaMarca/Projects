#include "persistency.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include "extra/textparser.h"

persistency* persistency::instance = NULL;

persistency::persistency()
{
	assert(instance == NULL);

	instance = this;

	this->current_max = 0;
}

void persistency::ReadLevelFile(const char* path)
{
	TextParser tp;
	tp.create(path);

	float t;
	this->current_max = tp.getint();

	char* w;

	while (w = tp.getword())
	{

		std::string str(w);
		if (str == "T") {
			this->times.push_back(tp.getfloat());
		}
	}
	std::cout << "LevelFile read" << std::endl;
}

void persistency::WriteLevelFile(const char* path)
{
	std::ofstream outfile(path);

	outfile << this->current_max << std::endl;

	for (int i = 0; i < this->times.size(); i++)
	{
		outfile << this->times[i] << std::endl;
	}

	outfile.close();
	std::cout << "Map saved at data/Maps/" + std::string(path) << std::endl;
}
