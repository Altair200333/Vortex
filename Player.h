#pragma once
#ifndef Player_h
#define Player_h
#include <stdio.h>
#include <GL/glew.h>
#include "baseShader.h"
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>

typedef struct {
	mat4 projection;
	mat4 view;
	vec3 eye;
	vec3 dir;
	vec3 up;
	float fov;
}Player;

Player initPlayer(float fov, int w, int h);
void recalculate(Player* pl);
void translate(Player* pl, vec3 dir);
void translateM(Player* pl, vec3 dir, float m);
void setProjectionView(Player* pl, Shader* s);

#endif