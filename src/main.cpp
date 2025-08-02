#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h> // has to be included first!
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "init.hpp"

//#define WIDTH 1024
//#define HEIGHT 1024
static const GLuint WIDTH = 1024, HEIGHT = 1024;

int main(int argc, char** argv) 
{

    GLFWwindow* window = initAndCreateWindow(WIDTH, HEIGHT, true);
    glViewport(0, 0, WIDTH, HEIGHT);

    while (glfwWindowShouldClose(window) == 0)
    {
        // clear the window
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.9f, -0.9f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.9f, -0.9f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.9f, 0.0f);
        glEnd();

        // swap buffer
        glfwSwapBuffers(window);

        // process user events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}