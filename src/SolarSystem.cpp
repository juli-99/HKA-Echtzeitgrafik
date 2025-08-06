#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include <filesystem>

#include "SolarSystem.hpp"


// Planetenskalierung relativ zueinander (nur Beispielwerte für Darstellung)
static constexpr struct PlanetData {
    const char* name;
    float dayLength;
    float orbitalSpeed;
    float distance;
    float scale;
    bool retrograde;
} planetData[] = {
    { "Sonne",   36000,   0.0f,    0.0f, 0.7f, false},
    { "Merkur",  84456, 47.87f,   57.9f, 0.2f, false},
    { "Venus",  349947, 35.02f,  108.2f, 0.4f, true},
    { "Erde",     1436, 29.78f,  149.6f, 0.5f, false},
    { "Mars",     1477, 24.08f,  227.9f, 0.3f, false},
    { "Jupiter",   595, 13.07f,  778.6f, 1.0f, false},
    { "Saturn",    647,  9.69f, 1433.5f, 0.9f, false},
    { "Uranus",   1034,  6.81f, 2872.5f, 0.8f, true},
    { "Neptun",    966,  5.43f, 4495.1f, 0.8f, false}
};

SolarSystem::SolarSystem(const std::filesystem::path& spherePath) : sharedGeometry(true) {
    loadMeshFromFile(spherePath);
    initPlanets();
}

SolarSystem::~SolarSystem() {}

void SolarSystem::loadMeshFromFile(const std::filesystem::path& spherePath)
{
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you’ll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(spherePath.string(),
        aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType | aiProcess_PreTransformVertices);

    // If the import failed, report it
    if (!scene) {
        std::cerr << "Importing of 3D scene failed: " << importer.GetErrorString() << std::endl;
        return;
    }
    if (!scene->HasMeshes()) return;

    aiMesh* mesh = scene->mMeshes[0]; // Just load the first mesh in this example

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

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
        // UV coordinates (if available)
        if (mesh->HasTextureCoords(0)) {
            aiVector3D& uv = mesh->mTextureCoords[0][j];
            vertices.push_back(uv.x); // U
            vertices.push_back(uv.y); // V
        }
        else {
            vertices.push_back(0.0f); // default U
            vertices.push_back(0.0f); // default V
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

    sharedGeometry.uploadVertexData(vertices.data(), vertices.size() * sizeof(float));
    sharedGeometry.uploadIndexData(indices.data(), indices.size() * sizeof(unsigned int));
    sharedGeometry.LinkAttrib(0, 3, GL_FLOAT, 8 * sizeof(GLfloat), (GLvoid*)0);
    sharedGeometry.LinkAttrib(1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    sharedGeometry.LinkAttrib(2, 2, GL_FLOAT, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
 
    sharedGeometry.setSizeVertex(vertices.size());
    sharedGeometry.setSizeIndices(indices.size());
}

void SolarSystem::initPlanets() {
    for (const auto& data : planetData) {
        planets.emplace_back(
            data.name,
            data.dayLength,
            data.orbitalSpeed,
            data.distance,
            data.scale,
            data.retrograde,
            &sharedGeometry
        );
    }
}

const std::vector<Planet>& SolarSystem::getPlanets() const {
    return planets;
}