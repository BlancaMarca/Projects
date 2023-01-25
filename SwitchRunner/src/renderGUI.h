#pragma once

#include "includes.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

class renderGUI
{
public:

	float window_width;
	float window_height;

	renderGUI();

	renderGUI(float w, float h);

	void renderGUIMenu(float x, float y, float w, float h, Texture* tex, float dt, bool flipuvs, bool f);


};
