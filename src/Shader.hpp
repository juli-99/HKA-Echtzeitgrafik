#pragma once
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <shared/data.h>

#define INFOLOG_LEN 512


class Shader {
public: 
	static GLint createShaderPipline(const char* vertexSource, const char* fragmentSource);
	static void run(GLint shaderProgram, GLFWwindow* window);
};