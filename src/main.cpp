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
#include "Planet.hpp"
#include "SolarSystem.hpp"
#include "PointLight.hpp"


bool isPerspective = true;
bool topView = false;
float distance = 9.0f;



void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    glm::mat4 model = glm::mat4(1.0f), view = glm::mat4(1.0f);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        isPerspective = !isPerspective;
    }

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        topView = !topView;
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    distance -= (float)yoffset;
    if (distance < 2.0f)
        distance = 2.0f;
    if (distance > 40.0f)
        distance = 40.0f;
}


std::filesystem::path fileFrag = fs::path(ROOT_DIR) / "res/shader.frag";
std::filesystem::path fileFragLight = fs::path(ROOT_DIR) / "res/pointLightShader.frag";
std::filesystem::path fileVertLight = fs::path(ROOT_DIR) / "res/pointLightShader.vert";
std::filesystem::path fileVert = fs::path(ROOT_DIR) / "res/shader.vert";

std::filesystem::path fileSphere = fs::path(ROOT_DIR) / "res/sphere.obj";


int main(int argc, char** argv) 
{
    const float distanceScale = 0.01f;
    const float orbitSpeedScale = 1.0f;
    const float rotationSpeedScale = 0.01f;

    std::cout << "Hello Projekt" << std::endl;

    GLFWwindow* window = initAndCreateWindow(true);

    glViewport(0, 0, WIDTH, HEIGHT); //Size of Window left -> to right/ 0 -> WIDTH
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

    //For the Light
    Shader lightShader;
    lightShader.createShaderPipline(fileFragLight, fileVertLight);
    
   

    Shader newShader;
    newShader.createShaderPipline(fileFrag, fileVert);

   
    //std::vector<float> vertices;
    //std::vector<unsigned int> indices;


    //loadMeshFromFile("sphere.obj", vertices, indices);


    

    /*Sending stuff between CPU und GPU with a Buffer Array (because it is really slow)*/
    /*GeometryBuffer buffer(true);

    buffer.uploadVertexData(vertices.data(), vertices.size() * sizeof(float));
    buffer.uploadIndexData(indices.data(), indices.size() * sizeof(unsigned int));

    
    buffer.LinkAttrib(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)0);
    buffer.LinkAttrib(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    */
    newShader.use();
    

    double prevTime = glfwGetTime();
    int nbFrames = 0;

    glm::vec3 viewPos;

    int lightDistance = 100;
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    SolarSystem solarSystem = SolarSystem(fileSphere);

    while (glfwWindowShouldClose(window) == 0)
    {
        float currTime = (float)glfwGetTime();

        nbFrames++;
        newShader.use();
        // clear the window and set Background color
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate matrices
        //glm::mat4 model = glm::mat4(1.0f);
     
        
      
    
        //view = glm::translate(view, -viewPos);
        // Move the camera backwards, so the objects becomes visible -> 95 basic without the line above
        glm::mat4  view = glm::mat4(1.0f);
        if (topView) {
            viewPos = glm::vec3(0.0f, -distance, 0.0f);
            view = glm::rotate(view, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            view = glm::translate(view, viewPos);
        }
        else {
            viewPos = glm::vec3(0.0f, -distance, -distance);
            view = glm::rotate(view, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            view = glm::translate(view, viewPos);
        }

        // Set perspective
        glm::mat4 projection;
        if (isPerspective)
            projection = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 0.1f, 1000.0f);
        else {
            float tmpDist = distance / 9; // fix scaling
            projection = glm::ortho(-4.0f * tmpDist, 4.0f * tmpDist, -3.0f * tmpDist, 3.0f * tmpDist, 0.1f, 1000.0f);
        }
        

        // Calculate matrices seins
        glm::mat4 model = glm::mat4(1.0f);
   
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    ;


        // Setting uniforms
        int modelLoc = glGetUniformLocation(newShader.getShaderProgram(), "u_model");
        newShader.setUniform(modelLoc, model);
     
        int viewLoc = glGetUniformLocation(newShader.getShaderProgram(), "u_view");
        newShader.setUniform(viewLoc, view);
 
        int perspectiveLoc = glGetUniformLocation(newShader.getShaderProgram(), "u_projection");
        newShader.setUniform(perspectiveLoc, projection);
    
        int viewPosLoc = glGetUniformLocation(newShader.getShaderProgram(), "u_viewPos");
        newShader.setUniform(viewPosLoc, viewPos);
      
        


        

        
        for (const Planet& planet : solarSystem.getPlanets()) {

            newShader.use();
            float angularvelocity_self = (2 * glm::pi<float>()) / (60 * planet.getDayLength());
            angularvelocity_self *= 1e7f; // to bring to same same scale as OrbitalSpeed
            if (planet.isRetrograde())
                angularvelocity_self = -angularvelocity_self;
            float angularvelocity_sun = planet.getOrbitalSpeed() / planet.getDistanceFromSun();
            if (std::isnan(angularvelocity_sun)) // if planet is sun
                angularvelocity_sun = 0;

            // Calculate matrices
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, angularvelocity_sun * orbitSpeedScale * currTime, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(planet.getDistanceFromSun() * distanceScale, 0.0f, 0.0f));
            model = glm::rotate(model, angularvelocity_self * rotationSpeedScale * currTime, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(planet.getScale()));

            // Setting uniforms
            newShader.setUniform(modelLoc, model);
            

            //Set Light Position

            lightShader.use();

            

            PointLight pointerLight;

            pointerLight.setModel(lightShader, model);
            pointerLight.setView(lightShader, view);
            pointerLight.setProjection(lightShader, projection);
            pointerLight.setViewPos(lightShader, viewPos);

            pointerLight.setPos(lightShader, lightPos);
            pointerLight.setPointLightConstant(lightShader, lightDistance);
            pointerLight.setColor(lightShader, lightColor);
            


          

            planet.getGeometry()->bind();
            glDrawElements(GL_TRIANGLES, planet.getGeometry()->getSizeIndices(), GL_UNSIGNED_INT, nullptr);
            planet.getGeometry()->unbind();
        }

     
     

        

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
    
