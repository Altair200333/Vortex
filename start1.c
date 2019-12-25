#define GLEW_STATIC
#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <ft2build.h>
#include FT_FREETYPE_H

//CustomHeaders
#include "baseShader.h"
#include "worldObject.h"
#include "renderManager.h"
#include "Player.h"
#include "textType.h"
#include "lightSource.h"
#include "Scene.h"
#include "physics.h"
#include "gizmos.h"
#include "level0.h"
#include <assert.h>

bool EdgeViewMode = false;

double posX1=-1, posY1=-1;
float Fspeed = 5;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float timeScale = 0.0f;

//Куда продавать душу за перегрузки и наследование?
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется	
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		if(timeScale == 0)
			timeScale = 1;
		else timeScale = 0;
	}
	
	if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
	{
		EdgeViewMode = !EdgeViewMode;
		if (EdgeViewMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	
}

//move player every frame(calback sucks)

int initialized = 0;
void addSphereObject(vec3 pos, Scene* scene)
{
	Object* obj = appendObject(scene->sceneObjects, generateSphere());
	translateGlobal(obj, pos);
	addObjectToWorld(scene->rigidBodyWorld, obj);
	initialized++;
}


//drag mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (posX1 == -1 || posX1 == -1)
	{
		posX1 = xpos;
		posY1 = ypos;
	}
	vec3 tr;
	glm_vec3_cross(pl->dir, pl->up, tr);

	glm_vec3_rotate(pl->dir, -(ypos - posY1)/1000, tr);
	glm_vec3_rotate(pl->up, -(ypos - posY1) / 1000, tr);

	glm_vec3_rotate(pl->dir, -(xpos - posX1) / 1000, (vec3) {0, 1, 0});
	glm_vec3_rotate(pl->up, -(xpos - posX1) / 1000, (vec3) { 0, 1, 0 });

	posX1 = xpos;
	posY1 = ypos;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}



void updateDeltaTime()
{
	float currentFrame = glfwGetTime();
	deltaTime = lastFrame==-1? 0 : (currentFrame - lastFrame)*timeScale;
	lastFrame = currentFrame;
}

int main()
{
	GLFWwindow* window = initWindow(windowWidth, windowHeight);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);
	
	Scene scene;
	initScene(&scene, onUpdate, onStart);
	scene.rigidBodyWorld = (RigidBodyWorld*)malloc(sizeof(RigidBodyWorld));
	RigidBodyWorldInit(scene.rigidBodyWorld);
	scene.sceneObjects = (ListObjects*)malloc(sizeof(ListObjects));
	scene.sceneObjects->count = 0;
	scene.sceneObjects->objects = 0;
	scene.onStart(scene);


	vec3 lightPos = { -6.0f, 4.0f, -3.2f };

	vec3 cubePositions[] = {
		{0.0f,  0.0f,  0.0f}, 
		{-5.0f,  -2.0f, -7.0f},
		{-1.5f, -2.2f, -2.5f},
		{-3.8f, -2.0f, -12.3f},
		{2.4f, 5.0f, -3.5f},
		{-1.7f,  -1.0f, -7.5f},
		{-1.7f, 1.0f, -7.5f},
		{1.5f,  2.0f, -2.5f},
		{1.5f,  0.2f, -1.5f},
		{-1.3f,  1.0f, -1.5f}
	};

	pl = initPlayer(45, windowWidth, windowHeight);

	//glEnable(GL_CULL_FACE);

	for (int i = 0; i < 9; i++)
	{
		appendObject(scene.sceneObjects, generateCube(1));
		translateGlobal((scene.sceneObjects->objects[i]), (float*)cubePositions[i]);
	}
	appendObject(scene.sceneObjects, fromStlFile("engDemo.stl"));
	translateGlobalV3(scene.sceneObjects->objects[scene.sceneObjects->count - 1], (Vector3) { 1, 0, -11 });
	
	Object* lightModels[1];
	lightModels[0] = generateCube(0.3f);
	setPos((lightModels[0]), lightPos);
	setShader((lightModels[0]), lightShader);
	
	appendObject(scene.sceneObjects,  generatePlane(1));
	glm_translate(scene.sceneObjects->objects[scene.sceneObjects->count-1]->model, (vec3) { 2, -4.0f, -2 });
	glm_scale(scene.sceneObjects->objects[scene.sceneObjects->count - 1]->model, (vec3) { 200.0f, 1.0f, 200.0f });
	rotateAxis(scene.sceneObjects->objects[scene.sceneObjects->count - 1], 90, (vec3) { 1.0f, 0.0f, 0.0f });
	
	appendObject(scene.sceneObjects, fromStlFile("cage.stl"));
	
	appendObject(scene.sceneObjects, fromStlFile("cage.stl"));
	
	
	
	LightSource* ls;
	LightSource* ps;
	
	ls = generateDirectionalLight((vec3) {-lightPos[0], -lightPos[1], -lightPos[2]},
		(vec3) {0.05f, 0.05f, 0.05f}, (vec3) { 0.4f, 0.4f, 0.4f },(vec3) {0.5f, 0.5f, 0.5f}, 0.71f);
	ps = generatePointLight((vec3) { lightPos[0], lightPos[1], lightPos[2] },	1.0f, 0.09, 0.032,(vec3) {0.05f, 0.05f, 0.05f},(vec3) {0.8f, 0.8f, 0.8f},(vec3) {1.0f, 1.0f, 1.0f});

	useShader(standartShader);
	setUpStandartShader();
	
	ListObjects lightList = { NULL, 0 };
	appendObject(scene.sceneObjects, lightModels[0]);


	LightList ll = { NULL, 0 };
	
	appendLigthSource(&ll, ls);
	appendLigthSource(&ll, ps);
	

	for(int i=0;i<10;i++)
	{
		addSphereObject((vec3) { -4+1.5*cos(i), 3+1.1*i, -2 +1.5*sin(i)}, &scene);
	}
	rotateAxis(scene.sceneObjects->objects[1], 30, (vec3) { 1, 0, 1 });
	addObjectToWorld(scene.rigidBodyWorld, scene.sceneObjects->objects[1]);

	err = fopen("logfile.txt", "w");

	lastFrame = -1;
	while (!glfwWindowShouldClose(window))
	{
		//printf("frame %d started\n", count);
		updateDeltaTime();

		//clear the screen
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//--DRAW
		movePlayer(pl, window, deltaTime, Fspeed, &scene);
		recalculateLookDirection(pl);
		
		//----------------
		//================= Calc shadows
		scene.onUpdate(scene);

		recalculateShadowsList(standartShader, ps, scene.sceneObjects);
		
		setProjectionView(pl, lightShader);
		setProjectionView(pl, standartShader);
	
		//--		
		
		setVec3( standartShader, "lightPos", 
			((PointLight*)(ps->lightSrc))->position[0],
			((PointLight*)(ps->lightSrc))->position[1],
			((PointLight*)(ps->lightSrc))->position[2]);

		
		renderListLights(standartShader, &ll);
				
		updatePhysicsWorld(scene.rigidBodyWorld, deltaTime);

		renderListObjects(scene.sceneObjects);

		renderListObjects(&lightList);

		glBindVertexArray(0);
		
		if(pl->selection!=NULL)
		{
			//gizmosDrawLineV3(vecToVector(pl.selection->position), add(vecToVector(pl.selection->position), (Vector3) { 0, 1, 0 }));
			Vector3 diff = sub(vecToVector(pl->selection->position), vecToVector(pl->eye));
			float dst = magnitude(diff);
			Vector3 ldir = vmul(normalized((vecToVector(pl->dir))), dst);
			
			Vector3 trgt = sub(ldir, diff);
			gizmosDrawLineV3(vecToVector(pl->selection->position), add(vecToVector(pl->selection->position), trgt));
			pl->selection->rigidBody.lineralVel = add(pl->selection->rigidBody.lineralVel, vmul(trgt, 3*deltaTime));
			//translateGlobalV3(pl.selection, vmul(trgt, 0.2 * deltaTime));
			pl->selection->rigidBody.lineralVel = add(pl->selection->rigidBody.lineralVel, vmul(pl->selection->rigidBody.lineralVel, -0.01));
		}
		//End Draw Calls
		glfwSwapBuffers(window);
		//printf("frame %d ended\n", count++);
	}
	//glDeleteVertexArrays(1, &containerVAO);
	//glDeleteBuffers(1, &VBO);
	glfwTerminate();
	glfwDestroyWindow(window);

	return 0;
}
