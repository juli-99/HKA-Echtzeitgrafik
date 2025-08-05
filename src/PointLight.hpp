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
    PointLight(Shader& shader);

    void setPos(glm::vec3 pos);
    void setDistance(int distance);
    void setColor(glm::vec3 color);

    void setModel(glm::mat4 model);
    void setView(glm::mat4 view);
    void setProjection(glm::mat4 projection);
    void setViewPos(glm::vec3 viewPos);
private:
    Shader shader;
};