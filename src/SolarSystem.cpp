#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

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
    { "Sonne",   36000,   0.0f,    0.0f, 3.0f, false},
    { "Merkur",  84456, 47.87f,   57.9f, 0.2f, false},
    { "Venus",  349947, 35.02f,  108.2f, 0.4f, true},
    { "Erde",     1436, 29.78f,  149.6f, 0.5f, false},
    { "Mars",     1477, 24.08f,  227.9f, 0.3f, false},
    { "Jupiter",   595, 13.07f,  778.6f, 1.0f, false},
    { "Saturn",    647,  9.69f, 1433.5f, 0.9f, false},
    { "Uranus",   1034,  6.81f, 2872.5f, 0.8f, true},
    { "Neptun",    966,  5.43f, 4495.1f, 0.8f, false}
};

SolarSystem::SolarSystem(const std::string& spherePath) {
    loadMesh(spherePath);
    initPlanets();
}

SolarSystem::~SolarSystem() {}

void SolarSystem::loadMesh(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
          aiProcess_CalcTangentSpace
        | aiProcess_Triangulate
        | aiProcess_JoinIdenticalVertices
        | aiProcess_SortByPType
        | aiProcess_PreTransformVertices
    );

    if (!scene) {
        std::cerr << "Importing of 3D scene failed: " << importer.GetErrorString() << std::endl;
    }

    processMesh(scene);
}

void SolarSystem::processMesh(const aiScene* scene) {
    const aiMesh* mesh = scene->mMeshes[0];

    std::vector<float> vertices;
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        // Position
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        // Normals (falls vorhanden)
        if (mesh->HasNormals()) {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        // Texture coords
        if (mesh->HasTextureCoords(0)) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }

    std::vector<unsigned int> indices;
    for (unsigned int k = 0; k < mesh->mNumFaces; k++) {
        aiFace& face = mesh->mFaces[k];
        // We can assume that there are only 3 indices per face as we set aiProcess_Triangulate for the import
        for (unsigned int j = 0; j < 3; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    sharedGeometry = GeometryBuffer(vertices, indices);
    //GeometryBuffer buffer = GeometryBuffer(false);
    //buffer.uploadVertexData();
    //buffer.uploadIndexData();
    //sharedGeometry = buffer;
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
