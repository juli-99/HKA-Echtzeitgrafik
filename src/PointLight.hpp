#pragma once
#define GLEW_STATIC
#include <GL/glew.h> // has to be included first!
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <ft2build.h>
#include "Shader.hpp"

#include <vector>
#include <map>



class PointLight {

public:
    void setPos(Shader shader, glm::vec3 pos);
    void setConstant(Shader shader, int distance);
    void setLin(Shader shader, int distance);
    void setQuad(Shader shader, int distance);
    void setColor(Shader shader, glm::vec3 color);

    void setModel(Shader shader, glm::mat4 model);
    void setView(Shader shader, glm::mat4 view);
    void setProjection(Shader shader, glm::mat4 projection);
    void setViewPos(Shader shader, glm::vec3 viewPos);
};