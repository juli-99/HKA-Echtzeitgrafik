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

#include <stb_image.h>

#include "helper/RootDir.h"
#include "init.hpp"

#include "Shader.hpp"
#include "GeometryBuffer.hpp"
#include "Planet.hpp"
#include "SolarSystem.hpp"
#include "PointLight.hpp"
#include "Fps.hpp"

static const GLuint WIDTH = 1024, HEIGHT = 1024;
static const float MIN_DISTANCE = 2.0f, MAX_DISTANCE = 40.0f, DEFAULT_DISTANCE = 9.0f;

static const std::filesystem::path fileFrag = fs::path(ROOT_DIR) / "res/shader.frag";
static const std::filesystem::path fileVert = fs::path(ROOT_DIR) / "res/shader.vert";
static const std::filesystem::path fileFragSun = fs::path(ROOT_DIR) / "res/sun.frag";
static const std::filesystem::path fileVertSun = fs::path(ROOT_DIR) / "res/sun.vert";
static const std::filesystem::path fileFragLight = fs::path(ROOT_DIR) / "res/pointLightShader.frag";
static const std::filesystem::path fileVertLight = fs::path(ROOT_DIR) / "res/pointLightShader.vert";
static const std::filesystem::path fileImage = fs::path(ROOT_DIR) / "res/textures/2k_sun.jpg";
static const std::filesystem::path fileSphere = fs::path(ROOT_DIR) / "res/sphere.obj";


bool isPerspective = true;
int selectView = 0;
float distance = DEFAULT_DISTANCE;

struct ImageData
{
    unsigned char* data;
    int width, height, nrChannels;
};

ImageData loadImage(std::filesystem::path imagePath)
{
    // Create Textures

    std::string texturePath = imagePath.string();

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* imageData = stbi_load(texturePath.data(), &width, &height, &nrChannels, 0);

    if (!imageData) {
        std::cerr << "Failed to load texture!" << std::endl;
    }

    return { imageData, width, height, nrChannels };
}

GLuint createTexture(ImageData imageData, int unit)
{

    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageData.width, imageData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    distance -= (float)yoffset;
    if (distance < MIN_DISTANCE)
        distance = MIN_DISTANCE;
    if (distance > MAX_DISTANCE)
        distance = MAX_DISTANCE;
}

int main(int argc, char** argv)
{
    const float distanceScale = 0.01f;
    const float orbitSpeedScale = 1.0f;
    const float rotationSpeedScale = 0.01f;

    GLFWwindow* window = initAndCreateWindow(WIDTH, HEIGHT, true);

    glViewport(0, 0, WIDTH, HEIGHT); //Size of Window left -> to right/ 0 -> WIDTH
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

    //Set Texture copy
    ImageData imageData = loadImage(fileImage);
    GLuint stoneTexture;
    if (imageData.data)
    {
        stoneTexture = createTexture(imageData, 0);
        stbi_image_free(imageData.data);
    }

    Shader shader(fileFragLight, fileVertLight);
    shader.use();

    const int modelLoc = shader.getUniformLoc("u_model");
    const int viewLoc = shader.getUniformLoc("u_view");
    const int perspectiveLoc = shader.getUniformLoc("u_projection");
    const int viewPosLoc = shader.getUniformLoc("u_viewPos");
    const int enablePointLightLoc = shader.getUniformLoc("u_enablePointLight");
    const int imageLoc = shader.getUniformLoc("u_image");


    int lightDistance = 100;
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    SolarSystem solarSystem = SolarSystem(fileSphere);

    Fps fps([](int fps) {std::cout << fps << std::endl; });
    fps.start();

    while (glfwWindowShouldClose(window) == 0)
    {
        float currTime = (float)glfwGetTime();

        // clear the window and set Background color
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Move the camera
        glm::mat4  view = glm::mat4(1.0f);
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
            shader.setUniform(imageLoc, 0);
            shader.setUniform(enablePointLightLoc, planet.getName() != "Sonne");

            PointLight pointerLight(shader);

            pointerLight.setModel(model);
            pointerLight.setPos(lightPos);
            pointerLight.setDistance(lightDistance);
            pointerLight.setColor(lightColor);


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