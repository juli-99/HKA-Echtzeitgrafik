#include "Fps.hpp"

Fps::Fps(std::function<void(int)> callback)
{
    this->callback = callback;
    this->nbFrames = 0;
    this->prevTime = -1;
}

void Fps::start() {
    this->nbFrames = 0;
    this->prevTime = glfwGetTime();
}

void Fps::stop() {
    this->prevTime = -1;
}

void Fps::countFrame() {
    double time = glfwGetTime();
    if (prevTime != -1) { // check if started
        this->nbFrames++;
        if (time - prevTime >= 1.0) {
            this->prevTime = time;

            this->callback(nbFrames);
            this->nbFrames = 0;
        }
    }
}