#pragma once
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

struct ImageData
{
    unsigned char* data;
    int width, height, nrChannels;
};



class Texture {
private:
    ImageData loadImage(std::filesystem::path imagePath);
    GLuint createTexture(ImageData imageData, int unit);
    int unitID; 
   

public:
    Texture(std::filesystem::path fileImage, int unit);
    int getUnitID() const;


};