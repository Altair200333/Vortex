#pragma once
#ifndef Player_h
#define Player_h
#include <stdio.h>
#include <GL/glew.h>
#include "baseShader.h"
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include "worldObject.h"
#include "GLFW/glfw3.h"
#include "Scene.h"

typedef struct {
	mat4 projection;
	mat4 view;
	vec3 eye;
	vec3 dir;
	vec3 up;
	float fov;
	Object* selection;
}Player;

Player* initPlayer(float fov, int w, int h);
void recalculateLookDirection(Player* pl);
void translatePlayer(Player* pl, vec3 dir);
void translateMagnitude(Player* pl, vec3 dir, float m);
void setProjectionView(Player* pl, Shader* s);
void movePlayer(Player* pl, GLFWwindow* window, float deltaTime, float Fspeed, Scene* scene);

#endif