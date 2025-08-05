#pragma once

#include <string>

#define GLEW_STATIC
#include <GL/glew.h> // has to be included first!
#include <GLFW/glfw3.h>

class Fps {
public:
	Fps(std::is_function<void(int)> callback);
	void start();
	void countFrame();
private:
	std::is_function<void(int)> callback;
	double prevTime;
	int nbFrames;
}