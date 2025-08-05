#include "Fps.hpp"

Fps::Fps(std::is_function<void(int)> callback)
{
    this->callback = callback;
}

void Fps::start() {
    this->prevTime = glfwGetTime();
}

void Fps::countFrame() {
    this->nbFrames++;
    double time = glfwGetTime();
    if (time - prevTime >= 1.0) {
        this->prevTime = time;

        this->callback(nbFrames);
        this->nbFrames = 0;
    }
}