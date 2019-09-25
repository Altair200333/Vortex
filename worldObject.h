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
	GLfloat* vertices;
	int vCount; 
	Shader* shader;
}Object;

Object generateCube(float scale);

#endif