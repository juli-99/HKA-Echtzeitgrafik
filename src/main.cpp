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
    distance -= (float) yoffset;
    if (distance < 2.0f)
        distance = 2.0f;
    if (distance > 40.0f)
        distance = 40.0f;
}

std::filesystem::path fileFrag = fs::path(ROOT_DIR) / "res/shader.frag";
std::filesystem::path fileVert = fs::path(ROOT_DIR) / "res/shader.vert";
std::filesystem::path fileFragLight = fs::path(ROOT_DIR) / "res/pointLightShader.frag";
std::filesystem::path fileVertLight = fs::path(ROOT_DIR) / "res/pointLightShader.vert";

std::filesystem::path fileSphere = fs::path(ROOT_DIR) / "res/sphere.obj";


int main(int argc, char** argv) 
{
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
    

    /*Sending stuff between CPU und GPU with a Buffer Array (because it is really slow)*/
    GeometryBuffer buffer(true);

    buffer.uploadVertexData(cubePhong, sizeof(cubePhong));
    buffer.LinkAttrib(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)0);
    buffer.LinkAttrib(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

//    newShader.use();
    

    double prevTime = glfwGetTime();
    int nbFrames = 0;


    const float distanceScale = 0.01f;
    const float orbitSpeedScale = 1.0f;
    const float rotationSpeedScale = 0.01f;

    const glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
    const glm::vec3 lightColour(1.0f, 1.0f, 1.0f);
    const int lightDistance = 50;

    // get uniform locations
    const int modelLoc = newShader.getUniformLoc("u_model");
    const int viewLoc = newShader.getUniformLoc("u_view");
    const int perspectiveLoc = newShader.getUniformLoc("u_projection");
    const int viewPosLoc = newShader.getUniformLoc("u_viewPos");

    PointLight pointLight(lightShader);

    SolarSystem solarSystem = SolarSystem(fileSphere);

    while (glfwWindowShouldClose(window) == 0)
    {
        float currTime = (float)glfwGetTime();

        // clear the window and set Background color
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Move the camera
        glm::mat4 view = glm::mat4(1.0f);
        glm::vec3 viewPos;
        if (topView) {
            viewPos = glm::vec3(0.0f, -distance, 0.0f);
            view = glm::rotate(view, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            view = glm::translate(view, viewPos);
        } else {
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

        // set uniforms that are identical for each planet
        lightShader.use();
        pointLight.setView(view);
        pointLight.setProjection(projection);
        pointLight.setViewPos(viewPos);

        pointLight.setPos(lightPos);
        pointLight.setDistance(lightDistance);
        pointLight.setColor(lightColour);

        newShader.setUniform(viewLoc, view);
        newShader.setUniform(perspectiveLoc, projection);
        newShader.setUniform(viewPosLoc, viewPos);

        for (const Planet& planet : solarSystem.getPlanets()) {
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
            pointLight.setModel(model);


            buffer.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);// Shape of primitiv;  Start of index Vertecies; Amount of Vertecies
            buffer.unbind();
        }

        // swap buffer -> back to front
        glfwSwapBuffers(window);

        // process user events
        glfwPollEvents();

        //FPS
        nbFrames++;
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