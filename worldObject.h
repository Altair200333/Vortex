#pragma once
#ifndef worldObject_h
#define worldObject_h
#include <stdio.h>
#include <GL/glew.h>
#include "baseShader.h"
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>

typedef struct
{
	GLuint VBO;
	GLuint VAO;
	mat4 model;
	vec3 location;
	GLfloat* vertices;
	int vCount; 
	Shader* shader;
	void(*render)(Object);
}Object;

void setShader(Object* obj, Shader* shader);
Object generateCube(float scale);
Object generatePlane(float scale);
Object fromStlFile(char* name);
void rotateAxis(Object* obj, float angle, vec3 axis);
#endif