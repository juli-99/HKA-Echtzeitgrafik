#include "Texture.hpp"
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

GLuint Texture::createTexture(ImageData imageData, int unit, GLint mipmapFilter, GLint textureWraper)
{

    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageData.width, imageData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWraper);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWraper);

    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmapFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

Texture::Texture(std::filesystem::path fileImage, int unit, GLint textureFilter, GLint textureWraper) {
    this->unitID = unit;
    ImageData imageData = loadImage(fileImage);
    if (imageData.data)
    {

        this->texture = createTexture(imageData, unit, textureFilter, textureWraper);
        stbi_image_free(imageData.data);
    }
}

Texture::~Texture()
{
    std::cout << this->texture << std::endl;
    glDeleteTextures(1, &(this->texture));

}

int Texture::getUnitID() const
{
    return this->unitID;
}

