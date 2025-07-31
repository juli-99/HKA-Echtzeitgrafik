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

float rectangle[] =
{
    // first triangle
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top right
     0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom right
    -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // top left 
    // second triangle
     0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top left
};

unsigned int indices[] =
{  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};




int main(int argc, char** argv) 
{
    std::cout << "Hello Projekt" << std::endl;

    GLFWwindow* window = initAndCreateWindow(true);

    glViewport(0, 0, WIDTH, HEIGHT); //Size of Window left -> to right/ 0 -> WIDTH
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   

    Shader newShader;

    newShader.createShaderPipline();

    /*Sending stuff between CPU und GPU with a Buffer Array (because it is really slow)*/
    GeometryBuffer buffer(true);

    buffer.uploadIndexData(indices, sizeof(indices));

    buffer.uploadVertexData(rectangle, sizeof(rectangle));
    buffer.LinkAttrib(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)0);
    buffer.LinkAttrib(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    newShader.use();

    double prevTime = glfwGetTime();
    int nbFrames = 0;

    while (glfwWindowShouldClose(window) == 0)
    {

        nbFrames++;

        // clear the window and set Background color
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        buffer.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);// Shape of primitiv;  Start of index Vertecies; Amount of Vertecies
        buffer.unbind();

        // swap buffer -> back to front
        glfwSwapBuffers(window);

        // process user events
        glfwPollEvents();

        //FPS -> Wieso nur 60 fps?
        double time = glfwGetTime();
        if (time - prevTime >= 1.0) {
            prevTime = time;
            
            std::cout << nbFrames << std::endl;
            nbFrames = 0;
        }
        
        
    }


    glfwDestroyWindow(window);
    glfwTerminate();
    
}
    