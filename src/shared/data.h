#pragma once

#include <GL/glew.h>

static const GLuint WIDTH = 1024, HEIGHT = 1024;
static const GLuint INFOLOG_LEN = 512;


static const GLchar* simpleVertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 ourColor;\n"
"void main() {\n"
" gl_Position = vec4(position, 1.0f);\n"
" ourColor = color;\n"
"}\0";

static const GLchar* simpleFragmentShaderSource =
"#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main() {\n"
" color = vec4(ourColor, 1.0f);\n"
"}\0";