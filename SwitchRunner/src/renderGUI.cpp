#include "renderGUI.h"

renderGUI::renderGUI() {
}

renderGUI::renderGUI(float w, float h) {
	window_width = w;
	window_height = h;
}

void renderGUI::renderGUIMenu(float x, float y, float w, float h, Texture* tex, float dt, bool flipuvs, bool f)
{
	Camera cam2D;
	cam2D.setOrthographic(0, window_width, window_height, 0, 1, -1);
	cam2D.enable();

	Mesh quad;
	quad.createQuad(x, y, w, h, flipuvs);

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	shader->setUniform("u_texture", tex, 0);

	Matrix44 quadModel;
	if (f == true)
	{
		quadModel.setTranslation(0, cos(dt) * 10.0f, 0);
	}

	shader->setUniform("u_model", quadModel);
	shader->setUniform("u_texture_tiling", 1.0f);

	quad.render(GL_TRIANGLES);
	shader->disable();
}