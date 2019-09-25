#pragma once
#ifndef renderManager_h
#define renderManager_h
#include <stdio.h>
#include <GL/glew.h>
#include "baseShader.h"
#include "worldObject.h"
#include <GLFW/glfw3.h>

typedef struct
{
	GLFWwindow* window;
	unsigned int VBO, VAO, EBO;
}Scene;

int initWindow(int w, int h);
int setupScene();

#endif