#pragma once
#ifndef renderManager_h
#define renderManager_h
#include <stdio.h>
#include <GL/glew.h>
#include "baseShader.h"
#include "worldObject.h"
#include <GLFW/glfw3.h>
#include "lightSource.h"

static unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

Shader* standartShader;

GLFWwindow* initWindow(int w, int h);
void renderObjectStandart(Object* obj);
void renderObjectCustom(Object* obj);
void renderObjectSpecificShader(Object* obj, Shader* sh);
void renderDirectionalLight(Shader* sh, struct LightSource* ls);
void renderPointLight(Shader* sh, struct LightSource* ls);
void renderSpotLight(Shader* sh, struct LightSource* ls);

void renderLights(Shader* sh, struct LightSource** ls, int size);

void recalculateShadows(Shader* s, struct LightSource* ls, Object* objects);
void renderScene(Shader* shader, Object* objects);

#endif