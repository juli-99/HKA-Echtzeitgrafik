#pragma once
#define GLEW_STATIC

#include <vector>
#include <GL/glew.h> // has to be included first!
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <ft2build.h>


class GeometryBuffer {

public:

    GeometryBuffer(bool useElementBuffer = false);
    GeometryBuffer(std::vector<float> &vertices, std::vector<unsigned int> &indices);
    ~GeometryBuffer();

    //Copy-Konstruktor deaktiviert
    GeometryBuffer(const GeometryBuffer&) = delete;
    //Copy-Assignment deaktiviert
    GeometryBuffer& operator=(const GeometryBuffer&) = delete;
    GeometryBuffer(GeometryBuffer&& other) noexcept;
    GeometryBuffer& operator=(GeometryBuffer&& other) noexcept;

    void uploadVertexData(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);
    void uploadIndexData(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

    void bind() const;
    void unbind() const;

    void LinkAttrib(GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr string, void* offset);

    GLuint getVAO() const { return vao_; }

    

private:
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
    bool hasEBO_;
};