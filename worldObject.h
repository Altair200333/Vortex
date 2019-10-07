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
	vec3 color;
	void(*render)(Object);
}Object;

typedef struct
{
	Object* objects;
	size_t count;
}ListObjects;

void appendObject(ListObjects* o, Object obj);
void setShader(Object* obj, Shader* shader);
Object generateCube(float scale);
Object generatePlane(float scale);
Object fromStlFile(char* name);
void rotateAxis(Object* obj, float angle, vec3 axis);
void setPos(Object* obj, vec3 axis);
#endif