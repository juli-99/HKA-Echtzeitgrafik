#include "Planet.hpp"
#include <glm/gtc/matrix_transform.hpp>

Planet::Planet(const std::string& name,
    float dayLengthMinutes,
    float orbitalSpeedKms,
    float distanceFromSunMillionKm,
    float scale,
    const GeometryBuffer& geometry)
    : name(name),
    dayLength(dayLengthMinutes),
    orbitalSpeed(orbitalSpeedKms),
    distanceFromSun(distanceFromSunMillionKm),
    scale(scale),
    retrograde(retrogradeRotation),
    geometry(geometry)
{
}

const std::string& Planet::getName() const {
    return name;
}
