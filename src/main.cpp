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


float cubePhong[] = {
    // Position           Normal
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,

    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

    // Left face
    -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,

    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

     // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,

    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
};

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




bool isPerspective = true;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    glm::mat4 model = glm::mat4(1.0f), view = glm::mat4(1.0f);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && isPerspective == false)
    {
        isPerspective = true;
    }
    else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS && isPerspective == true) {
        isPerspective = false;
    }
}


std::filesystem::path fileFrag = fs::path(ROOT_DIR) / "res/shader.frag";
std::filesystem::path fileFragLight = fs::path(ROOT_DIR) / "res/pointLightShader.frag";
std::filesystem::path fileVertLight = fs::path(ROOT_DIR) / "res/pointLightShader.vert";
std::filesystem::path fileVert = fs::path(ROOT_DIR) / "res/shader.vert";

int main(int argc, char** argv) 
{
    std::cout << "Hello Projekt" << std::endl;

    GLFWwindow* window = initAndCreateWindow(true);

    glViewport(0, 0, WIDTH, HEIGHT); //Size of Window left -> to right/ 0 -> WIDTH
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    //For the Light
    Shader lightShader;
    lightShader.createShaderPipline(fileFragLight, fileVertLight);
    
   

    Shader newShader;

    newShader.createShaderPipline(fileFrag, fileVert);
    

    /*Sending stuff between CPU und GPU with a Buffer Array (because it is really slow)*/
    GeometryBuffer buffer(true);

    //buffer.uploadIndexData(indices, sizeof(indices));

    buffer.uploadVertexData(cubePhong, sizeof(cubePhong));
    buffer.LinkAttrib(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)0);
    buffer.LinkAttrib(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    newShader.use();
    

    double prevTime = glfwGetTime();
    int nbFrames = 0;

    glm::vec3 viewPos(0.0f, 0.0f, 6.0f);
    glm::vec3 lightPos(0.0f, -2.0f, 3.0f); //x,y,z

    while (glfwWindowShouldClose(window) == 0)
    {

        nbFrames++;
        newShader.use();
        // clear the window and set Background color
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate matrices
        glm::mat4 model = glm::mat4(1.0f), view = glm::mat4(1.0f);
        glm::mat4 projection;
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

    
        //view = glm::translate(view, -viewPos);
        // Move the camera backwards, so the objects becomes visible -> 95 basic without the line above
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        if (isPerspective) {
            projection = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 0.1f, 1000.0f);
        }else 
        projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 1000.0f);

        // Setting uniforms
        int modelLoc = glGetUniformLocation(newShader.getShaderProgram(), "u_model");
        newShader.setUniform(modelLoc, model);
     
        int viewLoc = glGetUniformLocation(newShader.getShaderProgram(), "u_view");
        newShader.setUniform(viewLoc, view);
 
        int perspectiveLoc = glGetUniformLocation(newShader.getShaderProgram(), "u_projection");
        newShader.setUniform(perspectiveLoc, projection);
    
        int viewPosLoc = glGetUniformLocation(newShader.getShaderProgram(), "u_viewPos");
        newShader.setUniform(viewPosLoc, viewPos);
      
        


        //Set Light Position
        
        lightShader.use();//-> das muss irgendwie für das danach 
        struct PointLight {
            glm::vec3 pos;
            glm::vec3 color;
            float constant;
            float lin;
            float quad;
        };

        PointLight pointLight;
        pointLight.constant =  1.0;
        pointLight.lin = 0.09;
        pointLight.quad = 0.032;
        pointLight.pos = glm::vec3(0.0f, -2.0f, 3.0f);
        pointLight.color = glm::vec3(1.0f, 1.0f, 1.0f);

        int modelLoc2 = glGetUniformLocation(lightShader.getShaderProgram(), "u_model");
        lightShader.setUniform(modelLoc2, model);

        int viewLoc2 = glGetUniformLocation(lightShader.getShaderProgram(), "u_view");
        lightShader.setUniform(viewLoc2, view);

        int perspectiveLoc2 = glGetUniformLocation(lightShader.getShaderProgram(), "u_projection");
        lightShader.setUniform(perspectiveLoc2, projection);

        int viewPosLoc2 = glGetUniformLocation(lightShader.getShaderProgram(), "u_viewPos");
        lightShader.setUniform(viewPosLoc2, viewPos);

        int posLoc = glGetUniformLocation(lightShader.getShaderProgram(), "u_Light.pos");
        lightShader.setUniform(posLoc, pointLight.pos);
        
        int constLoc = glGetUniformLocation(lightShader.getShaderProgram(), "u_Light.constant");
        lightShader.setUniform(constLoc, pointLight.constant);

        int linLoc = glGetUniformLocation(lightShader.getShaderProgram(), "u_Light.lin");
        lightShader.setUniform(linLoc, pointLight.lin);

        int quadLoc = glGetUniformLocation(lightShader.getShaderProgram(), "u_Light.quad");
        lightShader.setUniform(quadLoc, pointLight.quad);


        int colorLoc = glGetUniformLocation(lightShader.getShaderProgram(), "u_Light.color");
        lightShader.setUniform(colorLoc, pointLight.color);
     
     

        buffer.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);// Shape of primitiv;  Start of index Vertecies; Amount of Vertecies
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
    
