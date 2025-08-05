#include "Fps.hpp"

Fps::Fps(std::function<void(int)> callback)
{
    this->callback = callback;
}

void Fps::start() {
    this->prevTime = glfwGetTime();
    this->nbFrames = 0;
}

void Fps::countFrame() {
    double time = glfwGetTime();
    this->nbFrames++;
    if (time - prevTime >= 1.0) {
        this->prevTime = time;

        this->callback(nbFrames);
        this->nbFrames = 0;
    }
}