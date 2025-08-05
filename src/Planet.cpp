#include "Planet.hpp"
#include <glm/gtc/matrix_transform.hpp>

Planet::Planet(const std::string& name,
    float dayLengthMinutes,
    float orbitalSpeedKms,
    float distanceFromSunMillionKm,
    float scale,
    bool retrogradeRotation,
    GeometryBuffer* geometry)
    : name(name),
    dayLength(dayLengthMinutes),
    orbitalSpeed(orbitalSpeedKms),
    distanceFromSun(distanceFromSunMillionKm),
    scale(scale),
    retrograde(retrogradeRotation),
        geometry(geometry)
{
        this->geometry = geometry;
}

const std::string& Planet::getName() const {
    return name;
}

float Planet::getDayLength() const {
    return dayLength;
}

float Planet::getOrbitalSpeed() const {
    return orbitalSpeed;
}

float Planet::getDistanceFromSun() const {
    return distanceFromSun;
}

float Planet::getScale() const {
    return scale;
}

bool Planet::isRetrograde() const {
    return retrograde;
}

GeometryBuffer* Planet::getGeometry() const {
   return geometry;
}