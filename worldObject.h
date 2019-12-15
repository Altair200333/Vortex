#pragma once
#ifndef worldObject_h
#define worldObject_h
#include <stdio.h>
#include <GL/glew.h>
#include "baseShader.h"
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include "vector3.h"

typedef struct _RigidBody
{
	Vector3 angluarVel;
	Vector3 lineralVel;
	Vector3 acceleration;
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
	vec3 position;
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
Object generateSphere();
Object generatePlane(float scale);
Object fromStlFile(char* name);
void rotateAxis(Object* obj, float angle, vec3 axis);
void rotateAxisV3(Object* obj, float angle, Vector3 axis);
void setPos(Object* obj, vec3 axis);
void translateLocal(Object* obj, vec3 shift);
void translateGlobal(Object* obj, vec3 shift);
void translateGlobalV3(Object* obj, Vector3 shift);
#endif