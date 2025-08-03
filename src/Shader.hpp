#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shared/data.h>

#include "helper/RootDir.h"

#define INFOLOG_LEN 512
namespace fs = std::filesystem;



class Shader {
private:
    GLint shaderProgram;
    

    
    static std::string loadShaderSource(std::string filepath);
public: 

    GLint getUniformLoc(const char* name);
	void createShaderPipline(std::filesystem::path FileFrag, std::filesystem::path FileVert);
    void setUniform(GLint location, int value);
    void setUniform(GLint location, float value);
    void setUniform(GLint location, const glm::vec3& value);
    void setUniform(GLint location, const glm::vec4& value);
    void setUniform(GLint location, const glm::mat4& value);
    void use();
    GLint getShaderProgram();
};