#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include <filesystem>

#include "SolarSystem.hpp"
#include "helper/RootDir.h"


// Planetenskalierung relativ zueinander (nur Beispielwerte für Darstellung)
static const struct PlanetData {
    const char* name;
    float dayLength;
    float orbitalSpeed;
    float distance;
    float scale;
    bool retrograde;
    fs::path filePath;
    int unitID;
} planetData[] = {
    { "Sonne",   36000,   0.0f,    0.0f, 0.9f, false, fs::path(ROOT_DIR) / "res/textures/2k_sun.jpg", 0},
    { "Merkur",  84456, 47.87f,   57.9f, 0.2f, false, fs::path(ROOT_DIR) / "res/textures/2k_mercury.jpg", 1},
    { "Venus",  349947, 35.02f,  108.2f, 0.4f, true,  fs::path(ROOT_DIR) / "res/textures/2k_venus.jpg", 2},
    { "Erde",     1436, 29.78f,  149.6f, 0.5f, false, fs::path(ROOT_DIR) / "res/textures/2k_earth.jpg", 3},
    { "Mars",     1477, 24.08f,  227.9f, 0.3f, false, fs::path(ROOT_DIR) / "res/textures/2k_mars.jpg", 4},
    { "Jupiter",   595, 13.07f,  778.6f, 1.0f, false, fs::path(ROOT_DIR) / "res/textures/2k_jupiter.jpg", 5},
    { "Saturn",    647,  9.69f, 1433.5f, 0.9f, false, fs::path(ROOT_DIR) / "res/textures/2k_saturn.jpg", 6},
    { "Uranus",   1034,  6.81f, 2872.5f, 0.8f, true,  fs::path(ROOT_DIR) / "res/textures/2k_uranus.jpg", 7},
    { "Neptun",    966,  5.43f, 4495.1f, 0.8f, false, fs::path(ROOT_DIR) / "res/textures/2k_neptune.jpg", 8},
    { "Moon",    1436,  39.78f, 199.6f, 0.135f, false, fs::path(ROOT_DIR) / "res/textures/2k_moon.jpg", 9 }
};

SolarSystem::SolarSystem(const std::filesystem::path& spherePath) : sharedGeometry(true)
{
    loadMeshFromFile(spherePath);
    initPlanets();
}

SolarSystem::~SolarSystem()
{
}

const std::vector<Planet>& SolarSystem::getPlanets() const
{
    return this->planets;
}

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
    if (!scene)
    {
        std::cerr << "Importing of 3D scene failed: " << importer.GetErrorString() << std::endl;
        return;
    }
    if (!scene->HasMeshes()) return;

    aiMesh* mesh = scene->mMeshes[0]; // Just load the first mesh in this example

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int j = 0; j < mesh->mNumVertices; j++)
    {
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
        else
        {
            vertices.push_back(0.0f); // default U
            vertices.push_back(0.0f); // default V
        }
    }

    for (unsigned int k = 0; k < mesh->mNumFaces; k++)
    {
        aiFace& face = mesh->mFaces[k];
        // Copy the index values to the vector "indices" above
        // We can assume that there are only 3 indices per face
        // because we set the aiProcess_Triangulate flag during the import.
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    this->sharedGeometry.uploadVertexData(vertices.data(), vertices.size() * sizeof(float));
    this->sharedGeometry.uploadIndexData(indices.data(), indices.size() * sizeof(unsigned int));
    this->sharedGeometry.LinkAttrib(0, 3, GL_FLOAT, 8 * sizeof(GLfloat), (GLvoid*)0);
    this->sharedGeometry.LinkAttrib(1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    this->sharedGeometry.LinkAttrib(2, 2, GL_FLOAT, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

    this->sharedGeometry.setSizeVertex(vertices.size());
    this->sharedGeometry.setSizeIndices(indices.size());
}

void SolarSystem::initPlanets()
{
    //this->planets.reserve(sizeof(planetData) / sizeof(planetData[0]));
    for (const auto& data : planetData)
    {
        this->planets.emplace_back(
            data.name,
            data.dayLength,
            data.orbitalSpeed,
            data.distance,
            data.scale,
            data.retrograde,
            &sharedGeometry,
            data.filePath,
            data.unitID

        );
    }
}