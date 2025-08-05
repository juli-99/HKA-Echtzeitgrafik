#pragma once

#include <functional>

#define GLEW_STATIC
#include <GL/glew.h> // has to be included first!
#include <GLFW/glfw3.h>

class Fps {
public:
	Fps(std::function<void(int)> callback);
	void start();
	void stop();
	void countFrame();
private:
	std::function<void(int)> callback;
	double prevTime;
	int nbFrames;
};