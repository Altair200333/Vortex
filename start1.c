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

bool EdgeViewMode = false;
Player pl;
double posX1=-1, posY1=-1;
float Fspeed = 5;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
//Куда продавать душу за перегрузки и наследование?
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
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
ListObjects list = { NULL, 0 };
RigidBodyWorld rw;
//move player every frame(calback sucks)
bool down = false, Rdown = false;

void addObject(Vector3 pos)
{
	appendObject(&list, fromStlFile("ico1.stl"));
	translateGlobalV3(&list.objects[list.count - 1], pos);
	addObjectToWorld(&rw, &(list.objects[list.count - 1]));
}
void addObjectVel(Vector3 pos, Vector3 initVel)
{
	appendObject(&list, fromStlFile("ico1.stl"));
	translateGlobalV3(&list.objects[list.count - 1], pos);
	addObjectToWorld(&rw, &(list.objects[list.count - 1]));
	for (int i = 0; i < 3; i++)
		list.objects[list.count - 1].rigidBody.lineralVel.axis[i] = initVel.axis[i];
}
void movePlayer(GLFWwindow* window)
{
	float speed = Fspeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		translateM(&pl, pl.dir, speed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		translateM(&pl, pl.dir, -speed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		vec3 tr;
		glm_vec3_cross(pl.dir, pl.up, tr);
		translateM(&pl, tr, speed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		vec3 tr;
		glm_vec3_cross(pl.dir, pl.up, tr);
		translateM(&pl, tr, -speed);

	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		translateM(&pl, pl.up, speed);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		translateM(&pl, pl.up, -speed);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !down)
	{
		down = true;
		printf("Down\n");

		pl.selection = castRay(vecToVector(pl.eye), vecToVector(pl.dir), &rw);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && down)
	{
		down = false;
		//printf("B\n");
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !Rdown)
	{
		Rdown = true;

		pl.selection = NULL;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && Rdown)
	{
		Rdown = false;
		//printf("B\n");
	}
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
	glm_vec3_cross(pl.dir, pl.up, tr);

	glm_vec3_rotate(pl.dir, -(ypos - posY1)/1000, tr);
	glm_vec3_rotate(pl.up, -(ypos - posY1) / 1000, tr);

	glm_vec3_rotate(pl.dir, -(xpos - posX1) / 1000, (vec3) {0, 1, 0});
	glm_vec3_rotate(pl.up, -(xpos - posX1) / 1000, (vec3) { 0, 1, 0 });

	posX1 = xpos;
	posY1 = ypos;

	if (glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT))
	{
		printf("AHTUNG!");
	}
}


void renderQuad(vec3 start, vec3 end)
{
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}




void initLights(vec3 lightPos, vec3 pointLightPositions[], LightSource** ls, LightSource** ps, LightSource** ps2, LightSource** ps3)
{
	*ls = generateDirectionalLight(
		(vec3){ -lightPos[0], -lightPos[1], -lightPos[2] },
		(vec3){ 0.05f, 0.05f, 0.05f }, (vec3){ 0.4f, 0.4f, 0.4f },
		(vec3){ 0.5f, 0.5f, 0.5f }, 0.71f);

	*ps = generatePointLight((vec3){ lightPos[0], lightPos[1], lightPos[2] }, 
	                         1.0f, 0.09, 0.032,
	                         (vec3) { 0.05f, 0.05f, 0.05f },
	                         (vec3) { 0.8f, 0.8f, 0.8f },
	                         (vec3) { 1.0f, 1.0f, 1.0f });

	*ps2 = generatePointLight((vec3) { pointLightPositions[1][0],
		                          pointLightPositions[1][1],
		                          pointLightPositions[1][2] }, 1.0f, 0.09, 0.032,
	                          (vec3) {0.05f, 0.05f, 0.05f}, 
	                          (vec3) { 0.8f, 0.8f, 0.8f },
	                          (vec3) { 1.0f, 1.0f, 1.0f });

	*ps3 = generatePointLight((vec3) { pointLightPositions[2][0],
		                          pointLightPositions[2][1],
		                          pointLightPositions[2][2] }, 1.0f, 0.09, 0.032,
	                          (vec3) {0.05f, 0.05f, 0.05f}, 
	                          (vec3) { 0.8f, 0.9f, 0.8f },
	                          (vec3) { 1.0f, 1.0f, 1.0f });
}



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	
}

int main()
{
	GLFWwindow* window = initWindow(windowWidth, windowHeight);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	unsigned long counter = 1;

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);
	
	
	standartShader = makeShader("vertexSh1.vs", "fragmentSh.fs");
	Shader* lightShader = makeShader("lightVertexShader.vs", "lightFragmentShader.fs");

	simpleDepthShader = makeShaderGeometry("point_shadows_depth.vs", "point_shadows_depth.fs", "point_shadows_depth.gs");

	//fromStlFile("dev.stl");
	vec3 lightPos = { -6.0f, 4.0f, -3.2f };

	vec3 cubePositions[] = {
		{0.0f,  0.0f,  0.0f}, 
		{2.0f,  5.0f, -15.0f},
		{-1.5f, -2.2f, -2.5f},
		{-3.8f, -2.0f, -12.3f},
		{2.4f, 5.0f, -3.5f},
		{-1.7f,  -1.0f, -7.5f},
		{-1.7f, 1.0f, -7.5f},
		{1.5f,  2.0f, -2.5f},
		{1.5f,  0.2f, -1.5f},
		{-1.3f,  1.0f, -1.5f}
	};
	
	vec3 pointLightPositions[] = {
		{lightPos[0], lightPos[1], lightPos[2]},
		{2.3f, -3.3f, -4.0f},
		{-4.0f,  6.0f, -12.0f},
	};
	pl = initPlayer(45, windowWidth, windowHeight);

	//glEnable(GL_CULL_FACE);

	for (unsigned int i = 0; i < 9; i++)
	{
		appendObject(&list, generateCube(1));
		translateGlobal(&(list.objects[i]), cubePositions[i]);
		//glm_translate(list.objects[i].model, cubePositions[i]);
	}

	Object lightModels[3];
	for (int i = 0; i < 3; i++)
	{
		lightModels[i] = generateCube(0.3f);
		//appendObject(&list, lightModels[i]);
		setPos(&(lightModels[i]), pointLightPositions[i]);
		//glm_scale(lightModels[i].model, (vec3) { 0.3, 0.3, 0.3 });
		setShader(&(lightModels[i]), lightShader);
	}
	RigidBodyWorldInit(&rw);
	
	appendObject(&list,  generatePlane(1));
	glm_translate(list.objects[list.count-1].model, (vec3) { 2, -4.0f, -2 });
	glm_scale(list.objects[list.count - 1].model, (vec3) { 200.0f, 1.0f, 200.0f });
	rotateAxis(&list.objects[list.count - 1], 90, (vec3) { 1.0f, 0.0f, 0.0f });
	
	appendObject(&list, fromStlFile("dev.stl"));
	appendObject(&list, fromStlFile("ico.stl"));
	glm_translate(list.objects[list.count - 1].model, (vec3) { -10, -2.9, -10.5 });

	appendObject(&list, fromStlFile("cage.stl"));
	setPos(&list.objects[list.count - 1], (vec3) { -5, -1, -4 });
	
	appendObject(&list, fromStlFile("cage.stl"));
	appendObject(&list, fromStlFile("ico1.stl"));
	translateGlobal(&list.objects[list.count - 1], (vec3) { -2.5, 3, -1.2 });
	//list.objects[list.count - 1].rigidBody.lineralVel[1] = -0.01;
	appendObject(&list, fromStlFile("ico1.stl"));
	translateGlobal(&list.objects[list.count - 1], (vec3) { -3, 0, -1 });

	appendObject(&list, fromStlFile("ico1.stl"));
	translateGlobal(&list.objects[list.count - 1], (vec3) { -3.5, 5, -1 });

	appendObject(&list, fromStlFile("ico1.stl"));
	translateGlobal(&list.objects[list.count - 1], (vec3) { -4.4, 6, -1 });
	
	addObject((Vector3) { -4, 7, -1 });
	addObjectVel((Vector3) { -4, 9, -1.1 }, (Vector3){0,0,1});
	addObjectVel((Vector3) { -3, 9, -1.1 }, (Vector3){1,0,-1});
	
	LightSource* ls;
	LightSource* ps;
	LightSource* ps2;
	LightSource* ps3;
	initLights(lightPos, pointLightPositions, &ls, &ps, &ps2, &ps3);


	SpotLight sli = { 
	{0, 2, -4},
	{0,-1,0},
	cos(GLM_PI / 180 * 10.5f),
	cos(GLM_PI / 180 * 17.0f), 
	1.0f, 0.09, 0.032,
	{0.03f, 0.03f, 0.03f}, 
	{0.6f, 0.6f, 0.6f},
	{1.0f, 1.0f, 1.0f} };
	LightSource sps = { TYPE_SPOT_LIGHT, &sli };
	initLight(&sps);

	LightSource* lights[] = {ls,ps, ps2, ps3};

	useShader(standartShader);
	setFloat(standartShader, "far_plane", far_plane);
	setVec3(standartShader, "material.ambient", 1.0f, 0.5f, 0.31f);
	setVec3(standartShader, "material.specular", 0.5f, 0.5f, 0.5f);
	setFloat(standartShader, "material.shininess", 16.0f);
	
	ListObjects lightList = { NULL, 0 };
	appendObject(&lightList, lightModels[0]);
	appendObject(&lightList, lightModels[1]);
	appendObject(&lightList, lightModels[2]);

	LightList ll = { NULL, 0 };
	for(int i=0;i<4;i++)
	{
		appendLigthSource(&ll, *(lights[i]));
	}
	
	Scene scene;
	scene.sceneObjects = list;
	scene.lights = ll;
	rotateAxis(&(list.objects[3]), 30.0, (vec3) { 0,0, 1 });
	rotateAxis(&(list.objects[3]), 30.0, (vec3) { 1,0, 0 });
	rotateAxis(&(list.objects[3]), 45.0, (vec3) { 0,1, 0 });
	
	//rotateAxis(&(list.objects[5]), 45.0, (vec3) { 1,0, 0 });
	//rotateAxis(&(list.objects[3]), 30.0, (vec3) { 1,0, 0 });
	list.objects[6].rigidBody.lineralVel.axis[1] = -0.01;

	
	addObjectToWorld(&rw, &(list.objects[14]));
	addObjectToWorld(&rw, &(list.objects[15]));
	addObjectToWorld(&rw, &(list.objects[16]));
	addObjectToWorld(&rw, &(list.objects[17]));
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		//printf("%f\n", deltaTime);
		
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearStencil(0); // this is the default value
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//--DRAW
		movePlayer(window);
		recalculate(&pl);
		
		//----------------
		//================= Calc shadows
		glm_vec3_rotate(((PointLight*)(ps->lightSrc))->position, 1*deltaTime,
			(vec3) { 0, 1, 0 });
		setPos(&(lightList.objects[0]), ((PointLight*)(ps->lightSrc))->position);

		recalculateShadowsList(standartShader, ps, &list);
		
		setProjectionView(&pl, lightShader);
		setProjectionView(&pl, standartShader);

		//--		
		
		setVec3( standartShader, "lightPos", 
			((PointLight*)(ps->lightSrc))->position[0],
			((PointLight*)(ps->lightSrc))->position[1],
			((PointLight*)(ps->lightSrc))->position[2]);

		
		renderListLights(standartShader, &ll);
		
		rotateAxis(&(list.objects[3]), 20*deltaTime, (vec3) { 0, 1, 0 });
		translateGlobal(&(list.objects[3]), (vec3) { 0, 0.005, 0 });


		computeCubeFall((Object*[]){  &(list.objects[4])}, 1, deltaTime);
		//computeSomething((Object*[]){  &(list.objects[15]), &(list.objects[14]), &(list.objects[16]), &(list.objects[17])}, 4, deltaTime);
		updatePhysicsWorld(&rw, deltaTime);
		//rotateAxis(&(objects[4]), 0.6f, (vec3) { 0.1, 0.2, -0.3 });
		renderListObjects(&list);
		
		
		renderListObjects(&lightList);

		glBindVertexArray(0);

		if(pl.selection!=NULL)
		{
			gizmosDrawLineV3(vecToVector(pl.selection->position), add(vecToVector(pl.selection->position), (Vector3) { 0, 1, 0 }));
			Vector3 diff = sub(vecToVector(pl.selection->position), vecToVector(pl.eye));
			float dst = magnitude(diff);
			Vector3 ldir = vmul(normalized((vecToVector(pl.dir))), dst);
			
			Vector3 trgt = sub(ldir, diff);
			gizmosDrawLineV3(vecToVector(pl.selection->position), add(vecToVector(pl.selection->position), trgt));
			pl.selection->rigidBody.lineralVel = add(pl.selection->rigidBody.lineralVel, vmul(trgt, 0.01));
		}
		//End Draw Calls
		glfwSwapBuffers(window);
	}
	//glDeleteVertexArrays(1, &containerVAO);
	//glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}
