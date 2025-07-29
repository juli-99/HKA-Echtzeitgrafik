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




    
    GLint shaderProgram = Shader::createShaderPipline();
   
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    /* Position attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    /* Color attribute */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glUseProgram(shaderProgram);

    while (glfwWindowShouldClose(window) == 0)
    {
        // clear the window
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // swap buffer
        glfwSwapBuffers(window);

        // process user events
        glfwPollEvents();
    }

    glfwTerminate();
    
    
}
    

    
   



    

    //std::filesystem::path FileFrag = "../../../res/shader.frag";
    //std::filesystem::path FileVert = "../../../res/shader.vert";
    
    //std::cout
      //  << "The default working directory is:\n"
        //<< std::filesystem::exists(FileFrag);

    /*
    std::cout << "Hello World!" << std::endl;


    GLFWwindow* window = initAndCreateWindow();
    glViewport(0, 0, WIDTH, HEIGHT);

    while (glfwWindowShouldClose(window) == 0)
    {
        // clear the window
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // swap buffer
        glfwSwapBuffers(window);

        // process user events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}*/