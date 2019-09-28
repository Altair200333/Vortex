#define GLEW_STATIC
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

bool EdgeViewMode = false;
Player pl;
double posX1=-1, posY1=-1;
float speed = 0.006f;


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
//move player every frame(calback sucks)
//TODO use delta time
void move(GLFWwindow* window)
{
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
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
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
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


int width = 1280, height = 720;
int main()
{
	GLFWwindow* window = initWindow(width, height);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	unsigned long counter = 1;

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);

	standartShader = makeShader("vertexSh1.vs", "fragmentSh.fs");
	Shader* lightShader = makeShader("lightVertexShader.vs", "lightFragmentShader.fs");

	Shader* simpleDepthShader = makeShaderGeometry("point_shadows_depth.vs", "point_shadows_depth.fs", "point_shadows_depth.gs");
	//fromStlFile("dev.stl");
	vec3 lightPos = { -2.0f, 4.0f, -3.2f };

	vec3 cubePositions[] = {
		{0.0f,  0.0f,  0.0f}, 
		{2.0f,  5.0f, -15.0f},
		{-1.5f, -2.2f, -2.5f},
		{-3.8f, -2.0f, -12.3f},
		{2.4f, -0.4f, -3.5f},
		{-1.7f,  3.0f, -7.5f},
		{1.3f, -2.0f, -2.5f},
		{1.5f,  2.0f, -2.5f},
		{1.5f,  0.2f, -1.5f},
		{-1.3f,  1.0f, -1.5f}
	};
	vec3 pointLightPositions[] = {
		{lightPos[0], lightPos[1], lightPos[2]},
		{2.3f, -3.3f, -4.0f},
		{-4.0f,  2.0f, -12.0f},
	};
	pl = initPlayer(45, width, height);

	//glEnable(GL_CULL_FACE);
	
	Object obj;
	obj = generateCube(1);
	glm_translate(obj.model, cubePositions[9]);
	rotateAxis(&obj, 45, (vec3) { 1, 0, 0 });

	Object objects[10];
	for (int i = 0; i < 9; i++)
	{
		objects[i] = generateCube(1);
		glm_translate(objects[i].model, cubePositions[i]);
	}

	Object lightModels[3];
	for (int i = 0; i < 3; i++)
	{
		lightModels[i] = generateCube(1);
		glm_translate(lightModels[i].model, pointLightPositions[i]);
		glm_scale(lightModels[i].model, (vec3) { 0.3, 0.3, 0.3 });
		setShader(&(lightModels[i]), lightShader);
	}

	Object plane;
	plane = generatePlane(1);
	glm_translate(plane.model, (vec3) { 2, -4.0f, -2 });
	glm_scale(plane.model, (vec3) { 10.0f, 1.0f, 10.0f });
	rotateAxis(&plane, 90, (vec3) { 1.0f, 0.0f, 0.0f });

	objects[9] = plane;
	
	LightSource* ls = generateDirectionalLight(
		(vec3){ -lightPos[0], -lightPos[1], -lightPos[2] },
		(vec3){ 0.05f, 0.05f, 0.05f }, (vec3){ 0.4f, 0.4f, 0.4f }, (vec3){ 0.5f, 0.5f, 0.5f }, 0.85f);
	PointLight pli = { {lightPos[0], lightPos[1], lightPos[2]}, 1.0f, 0.09, 0.032,
	{0.05f, 0.05f, 0.05f}, {0.8f, 0.8f, 0.8f}, {1.0f, 1.0f, 1.0f}};
	LightSource* ps = generatePointLight((vec3){ lightPos[0], lightPos[1], lightPos[2] }, 1.0f, 0.09, 0.032,
		(vec3) { 0.05f, 0.05f, 0.05f }, (vec3) { 0.8f, 0.8f, 0.8f }, (vec3) { 1.0f, 1.0f, 1.0f });

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

	LightSource lights[] = {ps};

	// configure depth map FBO
	// -----------------------
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	useShader(standartShader);
	setInt(setInt, "shadowMap", 0);
	useShader(simpleDepthShader);
	setInt(simpleDepthShader, "depthMap", 1);
	

	float near_plane = 1.0f;
	float far_plane = 45.0f;
	mat4 shadowProj;
	glm_perspective(GLM_PI/180.0f*90, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane, shadowProj);
	mat4 shadowTransforms[6];
	mat4 tmp, tmp3;
	vec3 tmp2;
	glm_vec3_add(lightPos, (vec3) { 1.0f, 0.0f, 0.0f }, tmp2);
	glm_lookat(lightPos, tmp2, (vec3) { 0.0f, -1.0f, 0.0f }, tmp);
	glm_mat4_mul(shadowProj, tmp, shadowTransforms[0]);

	glm_vec3_add(lightPos, (vec3) { -1.0f, 0.0f, 0.0f }, tmp2);
	glm_lookat(lightPos, tmp2, (vec3) { 0.0f, -1.0f, 0.0f }, tmp);
	glm_mat4_mul(shadowProj, tmp, shadowTransforms[1]);

	glm_vec3_add(lightPos, (vec3) { 0.0f, 1.0f, 0.0f }, tmp2);
	glm_lookat(lightPos, tmp2, (vec3) { 0.0f, 0.0f, 1.0f }, tmp);
	glm_mat4_mul(shadowProj, tmp, shadowTransforms[2]);

	glm_vec3_add(lightPos, (vec3) { 0.0f, -1.0f, 0.0f }, tmp2);
	glm_lookat(lightPos, tmp2, (vec3) { 0.0f, 0.0f, -1.0f }, tmp);
	glm_mat4_mul(shadowProj, tmp, shadowTransforms[3]);

	glm_vec3_add(lightPos, (vec3) { 0.0f, 0.0f, 1.0f }, tmp2);
	glm_lookat(lightPos, tmp2, (vec3) { 0.0f, -1.0f, 0.0f }, tmp);
	glm_mat4_mul(shadowProj, tmp, shadowTransforms[4]);

	glm_vec3_add(lightPos, (vec3) { 0.0f, 0.0f, -1.0f }, tmp2);
	glm_lookat(lightPos, tmp2, (vec3) { 0.0f, -1.0f, 0.0f }, tmp);
	glm_mat4_mul(shadowProj, tmp, shadowTransforms[5]);


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//--DRAW
		move(window);
		recalculate(&pl);
		
		//----------------
		//================= RENDER TO BUF
		recalculateShadows(simpleDepthShader, ps, objects);
		// reset viewport
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//---------
		//===========
		//RENDER NORMAL SENE
		useShader(standartShader);
		setProjectionView(&pl, standartShader);
		
		((PointLight*)(ps->lightSrc))->position[0] += 0.001;
		glm_translate(lightModels[0].model, (vec3) { -0.001, 0, 0 });
		setVec3( standartShader, "viewPos", pl.eye[0], pl.eye[1], pl.eye[2]);
		setVec3( standartShader, "lightPos", 
			((PointLight*)(ps->lightSrc))->position[0],
			((PointLight*)(ps->lightSrc))->position[1],
			((PointLight*)(ps->lightSrc))->position[2]);
		setFloat(standartShader, "far_plane", far_plane);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		renderLights(standartShader, lights, 1);
		
		setVec3(standartShader, "material.ambient", 1.0f, 0.5f, 0.31f);
		setVec3(standartShader, "material.diffuse", 1.0f, 0.5f, 0.31f);
		setVec3(standartShader, "material.specular", 0.5f, 0.5f, 0.5f);
		setFloat(standartShader,"material.shininess", 16.0f);
		
		for (unsigned int i = 0; i < 10; i++)
		{
			objects[i].render(&(objects[i]));
			//rotateAxis(&(objects[i]), 0.19f, (vec3) { (i+1)%10, i%10, 0 });
		}
		
		useShader(lightShader);
		setProjectionView(&pl, lightShader);
		for (unsigned int i = 0; i < 1; i++)
		{
			lightModels[i].render(&(lightModels[i]));
		}

		glBindVertexArray(0);

		counter++;
		//End Draw Calls
		glfwSwapBuffers(window);
	}
	//glDeleteVertexArrays(1, &containerVAO);
	//glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}