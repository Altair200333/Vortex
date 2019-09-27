#pragma once
#ifndef baseShader_h
#define baseShader_h
#include <stdio.h>
#include <GL/glew.h>; 
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
typedef struct 
{
	GLuint Program;
}Shader;

void useShader(Shader* shader);

Shader* makeShader(const GLchar* vertexPath, const GLchar* fragmentPath);
Shader* makeShaderGeometry(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath);
void setVec3(Shader* sh,char* name, float x, float y, float z);
void setVec4(Shader* sh,char* name, float x, float y, float z, float w);
void setMat4(Shader* sh,char* name, GLfloat* val);
void setFloat(Shader* sh,char* name, float x);
void setInt(Shader* sh, char* name, int x);
#endif