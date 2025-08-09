#include <glm/gtc/matrix_transform.hpp>

#include "Planet.hpp"

Planet::Planet(const std::string& name,
    float dayLengthMinutes,
    float orbitalSpeedKms,
    float distanceMillionKm,
    float scale,
    bool retrogradeRotation,
    int linkPlanet,
    GeometryBuffer* geometry,
    fs::path filePath,
    int unitID)
    : name(name),
    dayLength(dayLengthMinutes),
    orbitalSpeed(orbitalSpeedKms),
    distance(distanceMillionKm),
    scale(scale),
    retrograde(retrogradeRotation),
    linkPlanet(linkPlanet),
    geometry(geometry),
    texture(filePath, unitID, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE)
{
}

const std::string& Planet::getName() const
{
    return this->name;
}

float Planet::getDayLength() const
{
    return this->dayLength;
}

float Planet::getOrbitalSpeed() const
{
    return this->orbitalSpeed;
}

float Planet::getDistance() const
{
    return this->distance;
}

float Planet::getScale() const
{
    return this->scale;
}

bool Planet::getRetrograde() const
{
    return this->retrograde;
}

GeometryBuffer* Planet::getGeometry() const
{
   return this->geometry;
}

int Planet::getTextureUnit() const
{
    return this->texture.getUnitID();
}

int Planet::getlinkPlanet() const
{
    return this->linkPlanet;
}
