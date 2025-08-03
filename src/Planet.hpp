#pragma once

#include <string>
#include <glm/glm.hpp>
#include "GeometryBuffer.hpp"

class Planet {
public:
    Planet(const std::string& name,
        float dayLength_minutes,
        float orbitalSpeed_kms,
        float distanceFromSun_millionKm,
        float scale,
        const GeometryBuffer& geometry);

    const std::string& getName() const;

private:
    std::string name;
    float dayLength;              // in Minutes
    float orbitalSpeed;           // in km/s
    float distanceFromSun;        // in Millionen km
    float scale;                  // visueller Skalierungsfaktor
    GeometryBuffer geometry;
};
