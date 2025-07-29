#pragma once
#define GLEW_STATIC
#include <GL/glew.h> // has to be included first!
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <ft2build.h>


class GeometryBuffer {

public:

    GeometryBuffer(bool useElementBuffer = false);
    ~GeometryBuffer();

    void uploadVertexData(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);
    void uploadIndexData(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

    void bind() const;
    void unbind() const;

    GLuint getVAO() const { return vao_; }

private:
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
    bool hasEBO_;
};