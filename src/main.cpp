#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

#define GLEW_STATIC
#include <GL/glew.h> // has to be included first!
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "helper/RootDir.h"
#include "shared/data.h"
#include "shared/functions.h"

#include "Shader.hpp"
#include "GeometryBuffer.hpp"



GLfloat triangle[] =
{
    // Position           Color   
     0.9f, -0.9f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.9f, -0.9f, 0.0f,   0.0f, 1.0f, 0.0f,
     0.0f,  0.9f, 0.0f,   0.0f, 0.0f, 1.0f
}; //sonst mehrfach erzeugt über class




int main(int argc, char** argv) 
{
    std::cout << "HelloTriangleRetained" << std::endl;

    GLFWwindow* window = initAndCreateWindow();
    glViewport(0, 0, WIDTH, HEIGHT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   

    Shader newShader;

    newShader.createShaderPipline();

    GeometryBuffer buffer(false);

    buffer.uploadVertexData(triangle, sizeof(triangle));
    buffer.bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);

    glEnableVertexAttribArray(1); // Farbe
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    buffer.unbind();

   
    

    newShader.use();


    while (glfwWindowShouldClose(window) == 0)
    {
        // clear the window
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        buffer.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        buffer.unbind();

        // swap buffer
        glfwSwapBuffers(window);

        // process user events
        glfwPollEvents();
    }

    glfwTerminate();
    
    
}
    