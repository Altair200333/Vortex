#pragma once
#ifndef baseShader_h
#define baseShader_h
#include <stdio.h>
#include <GL/glew.h>; 

typedef struct 
{
	GLuint Program;
}Shader;

void useShader(Shader* shader);

Shader* makeShader(const GLchar* vertexPath, const GLchar* fragmentPath);
void setVec3(Shader* sh,char* name, float x, float y, float z);
void setFloat(Shader* sh,char* name, float x);
#endif