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
#include "init.hpp"

#include "Shader.hpp"
#include "GeometryBuffer.hpp"
#include "Planet.hpp"
#include "SolarSystem.hpp"
#include "PointLight.hpp"
#include "Fps.hpp"
#include "Texture.hpp"

static const GLuint WIDTH = 1024, HEIGHT = 1024;
static const float MIN_DISTANCE = 2.0f, MAX_DISTANCE = 40.0f, DEFAULT_DISTANCE = 6.0f;
static const float DISTANCE_SCALE = 0.01f, ORBIT_SPEED_SCALE = 1.0f, ROTATION_SPEED_SCALE = 0.01f;

static const std::filesystem::path SHADER_PATH_FRAG = fs::path(ROOT_DIR) / "res/pointLightShader.frag";
static const std::filesystem::path SHADER_PATH_VERT = fs::path(ROOT_DIR) / "res/pointLightShader.vert";
static const std::filesystem::path SPHERE_OBJ_PATH  = fs::path(ROOT_DIR) / "res/sphere.obj";


static bool isPerspective = true;
static int selectView = 0;
static float distance = DEFAULT_DISTANCE;

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    glm::mat4 model = glm::mat4(1.0f), view = glm::mat4(1.0f);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        isPerspective = !isPerspective;
    }
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        selectView += 1;
        if (selectView > 2)
            selectView = 0;
    }
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    distance -= (float)yoffset;
    if (distance < MIN_DISTANCE)
        distance = MIN_DISTANCE;
    if (distance > MAX_DISTANCE)
        distance = MAX_DISTANCE;
}

int main(int argc, char** argv)
{
    GLFWwindow* window = initAndCreateWindow(WIDTH, HEIGHT, true);

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);


    Shader shader(SHADER_PATH_FRAG, SHADER_PATH_VERT);
    shader.use();

    const int modelLoc = shader.getUniformLoc("u_model");
    const int viewLoc = shader.getUniformLoc("u_view");
    const int perspectiveLoc = shader.getUniformLoc("u_projection");
    const int viewPosLoc = shader.getUniformLoc("u_viewPos");
    const int enablePointLightLoc = shader.getUniformLoc("u_enablePointLight");
    const int imageLoc = shader.getUniformLoc("u_image");


    int lightDistance = 100; //TODO

    SolarSystem solarSystem = SolarSystem(SPHERE_OBJ_PATH);

    PointLight pointLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    Fps fps([](int fps) {std::cout << fps << std::endl; });
    fps.start();

    while (glfwWindowShouldClose(window) == 0)
    {
        float currTime = (float)glfwGetTime();

        // clear the window and set Background color
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Move the camera
        float viewAngle;
        glm::vec3 viewPos;
        switch (selectView) {
        default:
        case 0:
            // 0.7f aprogimation of sqrt(1/2) 
            // offsetting because distance of hypotenuse given
            viewPos = glm::vec3(0.0f, -distance * 0.7f, -distance * 0.7f);
            viewAngle = glm::radians(45.0f);
            break;
        case 1:
            viewPos = glm::vec3(0.0f, -distance, 0.0f);
            viewAngle = glm::radians(90.0f);
            break;
        case 2:
            viewPos = glm::vec3(0.0f, 0.0f, -distance);
            viewAngle = glm::radians(0.0f);
            break;
        }
        glm::mat4  view = glm::mat4(1.0f);
        view = glm::rotate(view, viewAngle, glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, viewPos);

        // Set perspective
        glm::mat4 projection;
        if (isPerspective)
            projection = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 0.1f, 1000.0f);
        else {
            float tmpDist = distance / 9; // fix scaling
            projection = glm::ortho(-4.0f * tmpDist, 4.0f * tmpDist, -3.0f * tmpDist, 3.0f * tmpDist, 0.1f, 1000.0f);
        }

        // Setting uniforms that don't change per Planet
        shader.setUniform(viewLoc, view);
        shader.setUniform(perspectiveLoc, projection);
        shader.setUniform(viewPosLoc, viewPos);

        for (const Planet& planet : solarSystem.getPlanets()) {

            float angularvelocity_self = (2 * glm::pi<float>()) / (60 * planet.getDayLength());
            angularvelocity_self *= 1e7f; // to bring to same same scale as OrbitalSpeed
            if (planet.getRetrograde())
                angularvelocity_self = -angularvelocity_self;
            float angularvelocity_sun = planet.getOrbitalSpeed() / planet.getDistanceFromSun();
            if (std::isnan(angularvelocity_sun)) // if planet is sun
                angularvelocity_sun = 0;

            // Calculate matrices
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, angularvelocity_sun * ORBIT_SPEED_SCALE * currTime, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(planet.getDistanceFromSun() * DISTANCE_SCALE, 0.0f, 0.0f));
            model = glm::rotate(model, angularvelocity_self * ROTATION_SPEED_SCALE * currTime, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(planet.getScale()));

            shader.setUniform(modelLoc, model);

            pointLight.applyToShader(shader, lightDistance);

            // Setting image
            shader.setUniform(imageLoc, planet.getTextureUnit());
            shader.setUniform(enablePointLightLoc, planet.getName() != "Sonne");


            planet.getGeometry()->bind();
            glDrawElements(GL_TRIANGLES, planet.getGeometry()->getSizeIndices(), GL_UNSIGNED_INT, nullptr);
            planet.getGeometry()->unbind();
        }


        // swap buffer -> back to front
        glfwSwapBuffers(window);

        // process user events
        glfwPollEvents();

        //FPS
        fps.countFrame();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}