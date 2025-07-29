#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <shared/data.h>

#include "helper/RootDir.h"

#define INFOLOG_LEN 512
namespace fs = std::filesystem;

class Shader {
private:
    
    static std::string loadShaderSource(std::string filepath);
public: 
	static GLint createShaderPipline();
    void setUniform(GLint location, int value);
    void setUniform(GLint location, float value);
    void setUniform(GLint location, const glm::vec3& value);
    void setUniform(GLint location, const glm::vec4& value);
    void setUniform(GLint location, const glm::mat4& value);
};