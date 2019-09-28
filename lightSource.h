#pragma once
#ifndef lightSource_h
#define lightSource_h
#include <stdio.h>
#include <GL/glew.h>
#include "baseShader.h"
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include "renderManager.h"

static const int TYPE_DIRECTIONAl_LIGHT = 0;
static const int TYPE_POINT_LIGHT = 1;
static const int TYPE_SPOT_LIGHT = 2;

static float near_plane = 1.0f;
static float far_plane = 45.0f;

typedef struct {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float strength;
}DirectionalLight;
typedef struct {
	unsigned int depthMapFBO;
	unsigned int depthCubemap;
	mat4 shadowTransforms[6];
}cubeShadowData;
typedef struct {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	cubeShadowData shadowData;
}PointLight;
typedef struct {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
}SpotLight;

typedef void(*renderFunc)(Shader* shader, struct LightSource* src);
//0 - dirLight
//1 - pointLight
//2 - spotLight
typedef struct {	
	int type;
	void* lightSrc;
	renderFunc render;
}LightSource;

void initLight(LightSource* src);
LightSource* generateDirectionalLight(vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular, float strength);
LightSource* generatePointLight(vec3 position, float constant,float linear,
float quadratic, vec3 ambient, vec3 diffuse, vec3 specular);

#endif