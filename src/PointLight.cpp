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

void PointLight::setPos(Shader shader, glm::vec3 pos)
{
	int posLoc = glGetUniformLocation(shader.getShaderProgram(), "u_Light.pos");
	shader.setUniform(posLoc, pos);
}

void PointLight::setPointLightConstant(Shader shader, int distance)
{

	int constLoc = glGetUniformLocation(shader.getShaderProgram(), "u_Light.constant");
	shader.setUniform(constLoc, attenuation.at(distance).at(0));

	int linLoc = glGetUniformLocation(shader.getShaderProgram(), "u_Light.lin");
	shader.setUniform(linLoc, attenuation.at(distance).at(1));

	int quadLoc = glGetUniformLocation(shader.getShaderProgram(), "u_Light.quad");
	shader.setUniform(quadLoc, attenuation.at(distance).at(2));
}

void PointLight::setColor(Shader shader, glm::vec3 color)
{
	int colorLoc = glGetUniformLocation(shader.getShaderProgram(), "u_Light.color");
	shader.setUniform(colorLoc, color);
}

void PointLight::setModel(Shader shader, glm::mat4 model)
{
	int modelLoc = glGetUniformLocation(shader.getShaderProgram(), "u_model");
	shader.setUniform(modelLoc, model);
}

void PointLight::setView(Shader shader, glm::mat4 view)
{
	int viewLoc = glGetUniformLocation(shader.getShaderProgram(), "u_view");
	shader.setUniform(viewLoc, view);
}

void PointLight::setProjection(Shader shader, glm::mat4 projection)
{
	int perspectiveLoc = glGetUniformLocation(shader.getShaderProgram(), "u_projection");
	shader.setUniform(perspectiveLoc, projection);
}

void PointLight::setViewPos(Shader shader, glm::vec3 viewPos)
{
	int viewPosLoc = glGetUniformLocation(shader.getShaderProgram(), "u_viewPos");
	shader.setUniform(viewPosLoc, viewPos);
}
