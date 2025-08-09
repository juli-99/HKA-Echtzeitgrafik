#include <glm/gtc/matrix_transform.hpp>

#include "Planet.hpp"

Planet::Planet(const std::string& name,
    float dayLengthMinutes,
    float orbitalSpeedKms,
    float distanceFromSunMillionKm,
    float scale,
    bool retrogradeRotation,
    GeometryBuffer* geometry,
    fs::path filePath,
    int unitID)
    : name(name),
    dayLength(dayLengthMinutes),
    orbitalSpeed(orbitalSpeedKms),
    distanceFromSun(distanceFromSunMillionKm),
    scale(scale),
    retrograde(retrogradeRotation),
    geometry(geometry),
    texture(filePath, unitID, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE)
{
}

//Deconstructor
Planet::~Planet() = default;

// Move constructor
Planet::Planet(Planet&& other) noexcept
    : name(std::move(other.name)),
    dayLength(other.dayLength),
    orbitalSpeed(other.orbitalSpeed),
    distanceFromSun(other.distanceFromSun),
    scale(other.scale),
    retrograde(other.retrograde),
    geometry(other.geometry),
    texture(std::move(other.texture))
{
    other.geometry = nullptr;
}

//Move Assignment
Planet& Planet::operator=(Planet&& other) noexcept {
    if (this != &other) {
        name = std::move(other.name);
        dayLength = other.dayLength;
        orbitalSpeed = other.orbitalSpeed;
        distanceFromSun = other.distanceFromSun;
        scale = other.scale;
        retrograde = other.retrograde;
        geometry = other.geometry;
        texture = std::move(other.texture);

        other.geometry = nullptr;
    }
    return *this;
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

float Planet::getDistanceFromSun() const
{
    return this->distanceFromSun;
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
