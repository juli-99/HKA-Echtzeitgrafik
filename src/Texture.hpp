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


#include "stb_image.h"

struct ImageData
{
    unsigned char* data;
    int width, height, nrChannels;
};


class Texture {
public:
    Texture(std::filesystem::path fileImage, int unit, GLint mipmapFilter, GLint textureWraper);
    ~Texture();
   

    // Delete copy semantics (no accidental copy of OpenGL handles)
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Move semantics
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    int getUnitID() const;

private:
    ImageData loadImage(std::filesystem::path imagePath);
    GLuint createTexture(ImageData imageData, int unit, GLint mipmapFilter, GLint textureWraper);
    int unitID; 
    GLuint texture;

    //Copy-Konstruktor deaktiviert
    //Texture(const Texture&) = delete;
    //Copy-Assignment deaktiviert
    //Texture& operator=(const Texture&) = delete;
    //Texture(Texture&& other) noexcept;
    //Texture& operator=(Texture&& other) noexcept;
};