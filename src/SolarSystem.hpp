#pragma once

#include <vector>
#include <memory>
#include <string>
#include <filesystem>
#include <assimp/scene.h>

#include "Planet.hpp"
#include "GeometryBuffer.hpp"

class SolarSystem {
public:
    SolarSystem(const std::filesystem::path& spherePath);
    ~SolarSystem();

    const std::vector<Planet>& getPlanets() const;

private:
    std::vector<Planet> planets;
    GeometryBuffer sharedGeometry;

    void loadMeshFromFile(const std::filesystem::path& spherePath);
    void initPlanets();
};