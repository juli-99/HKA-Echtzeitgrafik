#include "Texture.hpp"


ImageData Texture::loadImage(std::filesystem::path imagePath)
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

GLuint Texture::createTexture(ImageData imageData, int unit)
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

Texture::Texture(std::filesystem::path fileImage, int unit) {
    this->unitID = unit;
    ImageData imageData = loadImage(fileImage);
    GLuint stoneTexture;
    if (imageData.data)
    {
        stoneTexture = createTexture(imageData, unit);
        stbi_image_free(imageData.data);
    }
}

int Texture::getUnitID() const
{
    return this->unitID;
}

