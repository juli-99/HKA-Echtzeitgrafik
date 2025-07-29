#include "GeometryBuffer.hpp"


GeometryBuffer::GeometryBuffer(bool useElementBuffer)
    : vao_(0), vbo_(0), ebo_(0), hasEBO_(useElementBuffer)
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    if (hasEBO_)
        glGenBuffers(1, &ebo_);
}

GeometryBuffer::~GeometryBuffer() {
    if (hasEBO_)
        glDeleteBuffers(1, &ebo_);
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

void GeometryBuffer::uploadVertexData(const void* data, GLsizeiptr size, GLenum usage) {
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    glBindVertexArray(0);
}

void GeometryBuffer::uploadIndexData(const void* data, GLsizeiptr size, GLenum usage) {
    if (!hasEBO_)
        return;

    glBindVertexArray(vao_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    glBindVertexArray(0);
}

void GeometryBuffer::bind() const {
    glBindVertexArray(vao_);
}

void GeometryBuffer::unbind() const {
    glBindVertexArray(0);
}
