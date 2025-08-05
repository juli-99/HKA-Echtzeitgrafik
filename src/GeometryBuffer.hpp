#pragma once
#define GLEW_STATIC
#include <GL/glew.h> // has to be included first!
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <ft2build.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


class GeometryBuffer {

public:

    GeometryBuffer(bool useElementBuffer = false);
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

    int getSizeVertex();
    int getSizeIndices();

    void setSizeIndices(GLsizei indicesSize);
    void setSizeVertex(GLsizei vertexSize);

    

private:
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
    bool hasEBO_;

    GLsizei vertexSize;
    GLsizei indicesSize;

};