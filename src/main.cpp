#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h> // has to be included first!
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "helper/RootDir.h"
#include "init.hpp"
#include "SolarSystem.hpp"

//#define WIDTH 1024
//#define HEIGHT 1024
static const GLuint WIDTH = 1024, HEIGHT = 1024;

int main(int argc, char** argv) 
{
    std::cout << "before solarSystem init" << std::endl;

    SolarSystem solarSystem = SolarSystem(std::filesystem::path(ROOT_DIR)/"res/sphere.obj");

    std::cout << "after solarSystem init" << std::endl;

    for (const Planet& planet : solarSystem.getPlanets())
    {
        std::cout << planet.getName() << std::endl;
    }

    return 0;
}