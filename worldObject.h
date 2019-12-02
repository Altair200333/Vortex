#pragma once
#ifndef worldObject_h
#define worldObject_h
#include <stdio.h>
#include <GL/glew.h>
#include "baseShader.h"
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>

//assume thats cube for now
typedef struct _RigidBody
{
	vec3 lineralVel;
	vec3 acceleration;
	float mass;
	float theta;
	float omega;
	float alpha;
	float J;
}RigidBody;
typedef struct
{
	GLuint VBO;
	GLuint VAO;
	mat4 model;
	vec3 location;
	vec3 rotation;
	GLfloat* vertices;
	int vCount; 
	Shader* shader;
	vec3 color;
	void(*render)(Object);
	RigidBody rigidBody;
}Object;

typedef struct
{
	Object* objects;
	size_t count;
}ListObjects;


void initRigidBody(Object* obj);
void appendObject(ListObjects* o, Object obj);
void setShader(Object* obj, Shader* shader);
Object generateCube(float scale);
Object generatePlane(float scale);
Object fromStlFile(char* name);
void rotateAxis(Object* obj, float angle, vec3 axis);
void setPos(Object* obj, vec3 axis);
void translateLocal(Object* obj, vec3 shift);
void translateGlobal(Object* obj, vec3 shift);
#endif