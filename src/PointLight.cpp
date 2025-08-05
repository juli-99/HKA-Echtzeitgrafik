#include "PointLight.hpp"


std::map<int, std::vector<float>> attenuation= {
	{7, {1.0, 0.7, 1.8}},
	{13, {1.0, 0.35, 0.44}},
	{20, {1.0, 0.22,0.20}},
	{32, {1.0, 0.14, 0.07}},
	{50, {1.0, 0.09, 0.032}},
	{65, {1.0, 0.07, 0.017}},
	{100, {1.0, 0.045, 0.0075}},
	{160, {1.0, 0.027, 0.0028}},
	{200, {1.0, 0.022, 0.0019}},
	{325, {1.0, 0.014, 0.0007}},
};

PointLight::PointLight(Shader& shader)
{
	this->shader = shader;
}

void PointLight::setPos(glm::vec3 pos)
{
	int posLoc = shader.getUniformLoc("u_Light.pos");
	shader.setUniform(posLoc, pos);
}

void PointLight::setDistance(int distance)
{
	int constLoc = shader.getUniformLoc("u_Light.constant");
	shader.setUniform(constLoc, attenuation.at(distance).at(0));
	int linLoc = shader.getUniformLoc("u_Light.lin");
	shader.setUniform(linLoc, attenuation.at(distance).at(1));
	int quadLoc = shader.getUniformLoc("u_Light.quad");
	shader.setUniform(quadLoc, attenuation.at(distance).at(2));
}

void PointLight::setColor(glm::vec3 color)
{
	int colorLoc = shader.getUniformLoc("u_Light.color");
	shader.setUniform(colorLoc, color);
}

void PointLight::setModel(glm::mat4 model)
{
	int modelLoc = shader.getUniformLoc("u_model");
	shader.setUniform(modelLoc, model);
}

void PointLight::setView(glm::mat4 view)
{
	int viewLoc = shader.getUniformLoc("u_view");
	shader.setUniform(viewLoc, view);
}

void PointLight::setProjection(glm::mat4 projection)
{
	int perspectiveLoc = shader.getUniformLoc("u_projection");
	shader.setUniform(perspectiveLoc, projection);
}

void PointLight::setViewPos(glm::vec3 viewPos)
{
	int viewPosLoc = shader.getUniformLoc("u_viewPos");
	shader.setUniform(viewPosLoc, viewPos);
}
