#pragma once

#include <vector>
#include <memory>
#include <string>
#include <filesystem>
#include <assimp/scene.h>

#include "Planet.hpp"
#include "GeometryBuffer.hpp"
#include "helper/RootDir.h"

namespace fs = std::filesystem;


static const std::filesystem::path fileSun = fs::path(ROOT_DIR) / "res/textures/2k_sun.jpg";
static const std::filesystem::path fileJupiter = fs::path(ROOT_DIR) / "res/textures/2k_jupiter.jpg";
static const std::filesystem::path fileMars = fs::path(ROOT_DIR) / "res/textures/2k_mars.jpg";
static const std::filesystem::path fileMercury = fs::path(ROOT_DIR) / "res/textures/2k_mercury.jpg";
static const std::filesystem::path fileMoon = fs::path(ROOT_DIR) / "res/textures/2k_moon.jpg";
static const std::filesystem::path fileNeptune = fs::path(ROOT_DIR) / "res/textures/2k_neptune.jpg";
static const std::filesystem::path fileSaturn = fs::path(ROOT_DIR) / "res/textures/2k_saturn.jpg";
static const std::filesystem::path fileEarth = fs::path(ROOT_DIR) / "res/textures/2k_earth.jpg";
static const std::filesystem::path fileUranus = fs::path(ROOT_DIR) / "res/textures/2k_uranus.jpg";
static const std::filesystem::path fileVenus = fs::path(ROOT_DIR) / "res/textures/2k_venus.jpg";





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