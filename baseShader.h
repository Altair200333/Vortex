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
void setVec4(Shader* sh,char* name, float x, float y, float z, float w);
void setFloat(Shader* sh,char* name, float x);
void setInt(Shader* sh, char* name, int x);
#endif