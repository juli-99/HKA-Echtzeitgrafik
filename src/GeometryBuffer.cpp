#include "GeometryBuffer.hpp"


GeometryBuffer::GeometryBuffer(bool useElementBuffer)
    : vao_(0), vbo_(0), ebo_(0), hasEBO_(useElementBuffer)
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    if (hasEBO_)
        glGenBuffers(1, &ebo_);
}


//Move-Assignment
GeometryBuffer& GeometryBuffer::operator=(GeometryBuffer&& other) noexcept
{
    if (this != &other)
    {
        // Alte Ressourcen freigeben
        if (hasEBO_ && ebo_ != 0)
            glDeleteBuffers(1, &ebo_);
        if (vbo_ != 0)
            glDeleteBuffers(1, &vbo_);
        if (vao_ != 0)
            glDeleteVertexArrays(1, &vao_);

        // Neue übernehmen
        vao_ = other.vao_;
        vbo_ = other.vbo_;
        ebo_ = other.ebo_;
        hasEBO_ = other.hasEBO_;

        // Quelle leeren
        other.vao_ = 0;
        other.vbo_ = 0;
        other.ebo_ = 0;
        other.hasEBO_ = false;
    }

    return *this;
};

//Move-Konstruktor
GeometryBuffer::GeometryBuffer(GeometryBuffer&& other) noexcept
    : vao_(other.vao_), vbo_(other.vbo_), ebo_(other.ebo_), hasEBO_(other.hasEBO_)
{
    other.vao_ = 0;
    other.vbo_ = 0;
    other.ebo_ = 0;
    other.hasEBO_ = false;
}



GeometryBuffer::~GeometryBuffer() {
    if (hasEBO_ && ebo_ != 0)
        glDeleteBuffers(1, &ebo_);
    if (vbo_ != 0)
    glDeleteBuffers(1, &vbo_);
    if (vao_ != 0)
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

