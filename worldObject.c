#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <GL/glew.h>
#include "baseShader.h"
#include "worldObject.h"
#include "renderManager.h"
#include <string.h>

void initRigidBody(Object* obj)
{
	for (int i = 0; i < 3; i++)
		obj->rigidBody.acceleration[i] = 0;
	for (int i = 0; i < 3; i++)
		obj->rigidBody.lineralVel[i] = 0;
	obj->rigidBody.mass = 1;
	obj->rigidBody.alpha = 0;
	obj->rigidBody.theta = 0;
	obj->rigidBody.omega = 0;
	obj->rigidBody.J = 1;
}
Object generateCube(float scale)
{
	GLfloat vert[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	for(int i=0;i<36*6;i++)
	{
		vert[i] *= scale;
	}

	Object obj = {0,0, {  1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 }, {0,0,0},{0,0,0 }, vert, 36, NULL,
		{1.0f, 0.5f, 0.31f}, 0};
	initRigidBody(&obj);
	glGenVertexArrays(1, &obj.VAO);
	glGenBuffers(1, &obj.VBO);

	glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	glBindVertexArray(obj.VAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	obj.render = &renderObjectStandart;
	return obj;
}
void rotateAxis(Object* obj, float angle, vec3 axis)
{
	//glm_translate(obj->model, (vec3){-obj->location[0], -obj->location[1], -obj->location[2]});

	glm_rotate(obj->model, 3.1415926 / 180 * angle, axis);
	glm_euler_angles(obj->model, obj->rotation);
	//glm_translate(obj->model, obj->location);
}


void translateLocal(Object* obj, vec3 shift)
{
	glm_translate(obj->model,
		shift);
	glm_vec3_add(obj->location, shift, obj->location);
	
}

//yea, this works, but performance sucks for sure. For now i have no idea about more efficient methods.
void translateGlobal(Object* obj, vec3 shift)
{
	//current rotation in euler angles
	vec3 rot;
	glm_euler_angles(obj->model, rot);
	
	//matrix of current transform(rotation), is used to set rotation back to original
	mat4 mat;
	//inverse rotation matrix
	mat4 mat1;
	//get matrix
	glm_euler(rot, mat);
	//inverse it
	//this operation seems to be CPU expensive, should find more optimised solution
	glm_mat4_inv(mat, mat1);
	
	//rotate it in order to set object to default rotation with saving position and scale	
	glm_mat4_mul(obj->model, mat1, obj->model);
	//hell yea, translate it
	glm_translate(obj->model,
		shift);
	//set rotation back by right multiplication
	glm_mat4_mul(obj->model, mat, obj->model);
	
	//update position field
	glm_vec3_add(obj->location, shift, obj->location);
}

void setShader(Object* obj, Shader* shader)
{
	if (shader == NULL)
	{
		obj->shader = NULL;
		obj->render = renderObjectStandart;
		return;
	}
	obj->shader = shader;
	obj->render = renderObjectCustom;
}
Object generatePlane(float scale)
{
	GLfloat vert[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f
	};


	Object obj = { 0, 0, { 
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 }, {0,0,0},{0,0,0}, vert, 6, NULL ,
		{1.0f, 0.5f, 0.31f}};
	initRigidBody(&obj);
	glGenVertexArrays(1, &obj.VAO);
	glGenBuffers(1, &obj.VBO);

	glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	glBindVertexArray(obj.VAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	
	obj.render = &renderObjectStandart;
	return obj;
}
Object fromStlFile(char* name)
{
	
	FILE * fp = fopen(name, "r");
	if (fp == NULL) return;
	char c;
	int count = 0;

	long length = 0;
	while ((c = getc(fp)) != EOF)
	{
		if (c <= 127)
		{
			length++;
		}
	}
	GLfloat *vert = (GLfloat*)malloc(sizeof(GLfloat)*length);
	
	fseek(fp, 0, SEEK_SET);
	char x[1024];
	/* assumes no word exceeds length of 1023 */
	bool started = false;
	int iterator=0;
	float normal[3];
	float vertex[3];
	int CurSize = 0;
	while (fscanf(fp, " %1023s", x) == 1) {
		if (!started && strcmp(x, "facet") == 0)
		{
			started = true;
			//iterator--;
		}
		if(started)
		{
			if(strcmp(x, "facet") == 0)
			{
				iterator = 0;
			}
			//printf("%s ", x);

			if(iterator == 2)
			{
				sscanf(x, "%f", &(normal[0]));
			}
			else if (iterator == 3)
			{
				sscanf(x, "%f", &(normal[1]));
			}
			else if (iterator == 4)
			{
				sscanf(x, "%f", &(normal[2]));
			}
			else if (iterator == 8 || iterator == 12 || iterator == 16)
			{
				sscanf(x, "%f", &(vertex[0]));
			}
			else if (iterator == 9 || iterator == 13 || iterator == 17)
			{
				sscanf(x, "%f", &(vertex[1]));
			}
			else if (iterator == 10 || iterator == 14 || iterator == 18)
			{
				sscanf(x, "%f", &(vertex[2]));
				vert[CurSize] = vertex[0];
				vert[CurSize+1] = vertex[1];
				vert[CurSize+2] = vertex[2];
				vert[CurSize+3] = normal[0];
				vert[CurSize+4] = normal[1];
				vert[CurSize+5] = normal[2];
				CurSize += 6;
			}
			iterator++;

			if (strcmp(x, "endsolid") == 0)
			{
				break;
			}
		}
	}
	//for(int i=0; i<CurSize;i++)
	//{
	//	printf("%f ", vert[i]);
	//	if (i!=1 && (i+1) % 6 == 0)
	//		printf("\n");
	//}
	fclose(fp);

	Object obj = { 0, 0, {
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 }, {0,0,0},{0,0,0}, vert, CurSize/2, NULL };

	glGenVertexArrays(1, &obj.VAO);
	glGenBuffers(1, &obj.VBO);

	glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
	glBufferData(GL_ARRAY_BUFFER, CurSize*sizeof(float), vert, GL_STATIC_DRAW);

	glBindVertexArray(obj.VAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	obj.color[0] = 0.4f;
	obj.color[1] = 0.4f;
	obj.color[2] = 0.4f;
	obj.render = &renderObjectStandart;
	initRigidBody(&obj);
	return obj;
}
void setPos(Object* obj, vec3 axis)
{
	glm_translate(obj->model, 
		(vec3) {-obj->location[0],-obj->location[1], -obj->location[2]});

	glm_translate(obj->model,
		(vec3) {
		axis[0], axis[1], axis[2]
	});
	obj->location[0] = axis[0];
	obj->location[1] = axis[1];
	obj->location[2] = axis[2];
}
void appendObject(ListObjects* o, Object obj)
{
	if(o->objects == NULL)
	{
		o->objects = (Object*)malloc(sizeof(Object));
		o->count = 1;
		o->objects[0] = obj;
	}
	else
	{
		o->objects = (Object*)realloc(o->objects, sizeof(Object)*(o->count+1));
		o->objects[o->count] = obj;
		o->count++;
	}
}
