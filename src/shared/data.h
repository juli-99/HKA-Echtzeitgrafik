#pragma once

#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

typedef unsigned int GLuint;

static const GLuint WIDTH = 1024, HEIGHT = 1024;
static const GLuint INFOLOG_LEN = 512;

float cubePhong[] = {
    // Position           Normal
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,

    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

    // Left face
    -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,

    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

     // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,

    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
};

GLfloat triangle[] =
{
    // Position           Color   
     0.9f, -0.9f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.9f, -0.9f, 0.0f,   0.0f, 1.0f, 0.0f,
     0.0f,  0.9f, 0.0f,   0.0f, 0.0f, 1.0f
}; //sonst mehrfach erzeugt über class

float rectangle[] =
{
    // first triangle
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top right
     0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom right
    -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // top left 
    // second triangle
     0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top left
};

unsigned int indices[] =
{  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

void loadMeshFromFile(std::string fileName, std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    std::filesystem::path pathToFile(ROOT_DIR);
    pathToFile = pathToFile / "res" / fileName;

    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you’ll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(pathToFile.string(),
        aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType | aiProcess_PreTransformVertices);

    // If the import failed, report it
    if (!scene) {
        std::cerr << "Importing of 3D scene failed: " << importer.GetErrorString() << std::endl;
        return;
    }
    if (!scene->HasMeshes()) return;

    aiMesh* mesh = scene->mMeshes[0]; // Just load the first mesh in this example

    for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
        // Copy the vertex data into the vector "vertices"
        aiVector3D& v = mesh->mVertices[j];
        vertices.push_back(v.x);
        vertices.push_back(v.y);
        vertices.push_back(v.z);

        if (mesh->HasVertexColors(0))
        {
            // Use vertex colors if they are available
            aiColor4D& c = mesh->mColors[0][j];
            vertices.push_back(c.r);
            vertices.push_back(c.g);
            vertices.push_back(c.b);
        }
        else
        {
            // In this case we use the position as the color attribute
            vertices.push_back(v.x);
            vertices.push_back(v.y);
            vertices.push_back(v.z);
        }
    }

    for (unsigned int k = 0; k < mesh->mNumFaces; k++) {
        aiFace& face = mesh->mFaces[k];
        // Copy the index values to the vector "indices" above
        // We can assume that there are only 3 indices per face
        // because we set the aiProcess_Triangulate flag during the import.
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
}