#pragma once
#ifndef renderManager_h
#define renderManager_h
#include <stdio.h>
#include <GL/glew.h>
#include "baseShader.h"
#include "worldObject.h"
#include <GLFW/glfw3.h>
#include "lightSource.h"

Shader* standartShader;

GLFWwindow* initWindow(int w, int h);
void renderObjectStandart(Object* obj);
void renderObjectCustom(Object* obj);
void renderDirectionalLight(Shader* sh, struct LightSource* ls, int id);
void renderPointLight(Shader* sh, struct LightSource* ls, int id);
void renderSpotLight(Shader* sh, struct LightSource* ls, int id);

#endif