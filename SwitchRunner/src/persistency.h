#pragma once
#include "framework.h"

#define MAX_LEVEL = 3


class persistency
{
public:

static persistency* instance;

int current_max;
std::vector<float> times;

persistency();

void ReadLevelFile(const char* path);

void WriteLevelFile(const char* path);
};

