#include <iostream>
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




namespace fs = std::filesystem;

int main(int argc, char** argv) 
{

    std::cout << "HelloTriangleRetained" << std::endl;

    GLFWwindow* window = initAndCreateWindow();
    glViewport(0, 0, WIDTH, HEIGHT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    
    GLint shaderProgram = Shader::createShaderPipline(simpleVertexShaderSource, simpleFragmentShaderSource);
    Shader::run(shaderProgram, window);
    
    
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