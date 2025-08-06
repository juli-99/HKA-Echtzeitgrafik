#include "GeometryBuffer.hpp"




//vbo -> creates Buffer with data
//vao -> pointer on one or more Buffers

GeometryBuffer::GeometryBuffer(bool useElementBuffer)
    : vao(0), vbo(0), ebo(0), eboActive(useElementBuffer), vertexSize(0), indicesSize(0)
{
    glGenVertexArrays(1, &vao);
  
    glGenBuffers(1, &vbo); //1 for 1 Buffer for the Object, Reference

    if (eboActive) {
        glGenBuffers(1, &ebo);
        
    }
}


//Move-Assignment
GeometryBuffer& GeometryBuffer::operator=(GeometryBuffer&& other) noexcept
{
    if (this != &other)
    {
        // Alte Ressourcen freigeben
        if (this->eboActive && this->ebo != 0)
            glDeleteBuffers(1, &this->ebo);
        if (this->vbo != 0)
            glDeleteBuffers(1, &this->vbo);
        if (this->vao != 0)
            glDeleteVertexArrays(1, &this->vao);

        // Neue übernehmen
        this->vao = other.vao;
        this->vbo = other.vbo;
        this->ebo = other.ebo;
        this->eboActive = other.eboActive;

        // Quelle leeren
        other.vao = 0;
        other.vbo = 0;
        other.ebo = 0;
        other.eboActive = false;
    }

    return *this;
};


//Move-Konstruktor
GeometryBuffer::GeometryBuffer(GeometryBuffer&& other) noexcept
    : vao(other.vao), vbo(other.vbo), ebo(other.ebo), eboActive(other.eboActive), vertexSize(other.vertexSize), indicesSize(other.indicesSize)
{
    other.vao = 0;
    other.vbo = 0;
    other.ebo = 0;
    other.eboActive = false;

}



GeometryBuffer::~GeometryBuffer() {
    if (this->eboActive && this->ebo != 0)
        glDeleteBuffers(1, &this->ebo);
    if (this->vbo != 0)
    glDeleteBuffers(1, &this->vbo);
    if (this->vao != 0)
    glDeleteVertexArrays(1, &this->vao);
}

void GeometryBuffer::uploadVertexData(const void* data, GLsizeiptr size, GLenum usage) {

    vertexSize = size;

    /*We create a Buffer, everytime we fire a function that modify data of the object -> it will change the binded object */
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    glBindVertexArray(0);
}

void GeometryBuffer::uploadIndexData(const void* data, GLsizeiptr size, GLenum usage) {
    if (!this->eboActive)
        return;

    this->indicesSize = size;
    

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    glBindVertexArray(0);
}

void GeometryBuffer::bind() const {
    glBindVertexArray(this->vao);
}

void GeometryBuffer::unbind() const {
    glBindVertexArray(0);
}

void GeometryBuffer::LinkAttrib(GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr string, void* offset)
{
    glBindVertexArray(this->vao);
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, string, offset); //Position of Vertex Attribute; Values per Vertex; Which Values Type; Coordinates as int; Data between each Vertex; Offset
    glEnableVertexAttribArray(layout);// Position of Vertex attribute
    glBindVertexArray(0);
}

GLsizei GeometryBuffer::getSizeVertex()
{
    return this->vertexSize;
}

GLsizei GeometryBuffer::getSizeIndices()
{
    return this->indicesSize;
}

void GeometryBuffer::setSizeIndices(GLsizei size) {
    this->vertexSize = size;
}

void GeometryBuffer::setSizeVertex(GLsizei size) {
    this->indicesSize = size;
}

