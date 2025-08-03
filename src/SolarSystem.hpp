#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Planet.hpp"
#include "GeometryBuffer.hpp"
#include <assimp/scene.h>

class SolarSystem {
public:
    SolarSystem(const std::string& spherePath);
    ~SolarSystem();

    const std::vector<Planet>& getPlanets() const;

private:
    std::vector<Planet> planets;
    GeometryBuffer sharedGeometry;

    void loadMesh(const std::string& path);
    void initPlanets();
    void processMesh(const aiScene* scene);
};
