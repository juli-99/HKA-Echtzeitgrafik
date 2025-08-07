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


#include "helper/RootDir.h"

namespace fs = std::filesystem;



class Shader {
public: 
    Shader(const std::filesystem::path FileFrag, const std::filesystem::path FileVert);
	
    void setUniform(GLint location, bool value);
    void setUniform(GLint location, int value);
    void setUniform(GLint location, float value);
    void setUniform(GLint location, const glm::vec3& value);
    void setUniform(GLint location, const glm::vec4& value);
    void setUniform(GLint location, const glm::mat4& value);
    GLint getUniformLoc(const char* name) const;
    GLint getShaderProgram() const;
    void use();
private:
    static const int INFOLOG_LEN;
    GLint shaderProgram;
    
    static std::string loadShaderSource(std::string filepath);
};