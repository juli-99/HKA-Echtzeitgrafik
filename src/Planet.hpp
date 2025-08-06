#pragma once

#include <string>
#include <filesystem>
#include <glm/glm.hpp>

#include "GeometryBuffer.hpp"
#include "Texture.hpp"

namespace fs = std::filesystem;

class Planet {
public:
    Planet(const std::string& name,
        float dayLength_minutes,
        float orbitalSpeed_kms,
        float distanceFromSun_millionKm,
        float scale,
        bool retrogradeRotation,
        GeometryBuffer* geometry,
        fs::path filePath,
        int unitID);

    const std::string& getName() const;
    float getDayLength() const;
    float getOrbitalSpeed() const;
    float getDistanceFromSun() const;
    float getScale() const;
    bool  getRetrograde() const;
    GeometryBuffer* getGeometry() const;
    int getTextureUnit() const;
    
private:
    std::string name;
    float dayLength;              // in Minutes
    float orbitalSpeed;           // in km/s
    float distanceFromSun;        // in Millionen km
    float scale;                  // visueller Skalierungsfaktor
    bool retrograde;
    GeometryBuffer* geometry;
    Texture texture;
};