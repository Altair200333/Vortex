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

#endif