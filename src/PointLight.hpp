#pragma once

#include <array>
#include <map>
#define GLEW_STATIC
#include <GL/glew.h> // has to be included first!
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <ft2build.h>

#include "Shader.hpp"


class PointLight {

public:
    PointLight();
    PointLight(glm::vec3 pos, glm::vec3 color);

    void setPos(glm::vec3 pos);
    void setColor(glm::vec3 color);
    glm::vec3 getPos() const;
    glm::vec3 getColor() const;

    void applyToShader(Shader& shader, int distance);

private:
    glm::vec3 pos;
    glm::vec3 color;
    static const std::map<int, const std::array<const float, 3>> attenuation;
};