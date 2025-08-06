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

PointLight::PointLight()
{
	this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
	this->color = glm::vec3(1.0f, 1.0f, 1.0f);
}

PointLight::PointLight(glm::vec3 pos, glm::vec3 color)
{
	this->pos = pos;
	this->color = color;
}

void PointLight::setPos(glm::vec3 pos)
{
	this->pos = pos;
}

void PointLight::setColor(glm::vec3 color)
{
	this->color = color;
}

glm::vec3 PointLight::getPos()
{
	return this->pos;
}

glm::vec3 PointLight::getColor()
{
	return this->color;
}

void PointLight::shader(Shader shader, int distance)
{
	int constLoc = shader.getUniformLoc("u_Light.constant");
	shader.setUniform(constLoc, attenuation.at(distance).at(0));
	int linLoc = shader.getUniformLoc("u_Light.lin");
	shader.setUniform(linLoc, attenuation.at(distance).at(1));
	int quadLoc = shader.getUniformLoc("u_Light.quad");
	shader.setUniform(quadLoc, attenuation.at(distance).at(2));
}