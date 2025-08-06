#include "GeometryBuffer.hpp"




//vbo -> creates Buffer with data
//vao -> pointer on one or more Buffers

GeometryBuffer::GeometryBuffer(bool useElementBuffer)
    : vao_(0), vbo_(0), ebo_(0), hasEBO_(useElementBuffer)
{
    glGenVertexArrays(1, &vao_);
  
    glGenBuffers(1, &vbo_); //1 for 1 Buffer for the Object, Reference

    if (hasEBO_) {
        glGenBuffers(1, &ebo_);
        
    }
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

    vertexSize = size;

    /*We create a Buffer, everytime we fire a function that modify data of the object -> it will change the binded object */
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    glBindVertexArray(0);
}

void GeometryBuffer::uploadIndexData(const void* data, GLsizeiptr size, GLenum usage) {
    if (!hasEBO_)
        return;

    indicesSize = size;
    

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

void GeometryBuffer::LinkAttrib(GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr string, void* offset)
{
    glBindVertexArray(vao_);
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, string, offset); //Position of Vertex Attribute; Values per Vertex; Which Values Type; Coordinates as int; Data between each Vertex; Offset
    glEnableVertexAttribArray(layout);// Position of Vertex attribute
    glBindVertexArray(0);

    /*
    * 
    buffer.bind();

    glEnableVertexAttribArray(0);// Position of Vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); //Position of Vertex Attribute; Values per Vertex; Which Values Type; Coordinates as int; Data between each Vertex; Offset

    /*Color
    glEnableVertexAttribArray(1); // Position of Vertex attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    buffer.unbind();

    */
    

}

GLsizei GeometryBuffer::getSizeVertex()
{
    return vertexSize;
}

GLsizei GeometryBuffer::getSizeIndices()
{
    return indicesSize;
}

void GeometryBuffer::setSizeIndices(GLsizei size) {
    this->vertexSize = size;
}

void GeometryBuffer::setSizeVertex(GLsizei size) {
    this->indicesSize = size;
}

