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



float deltaTime = 0.0f;
float lastFrame = 0.0f;
float timeScale = 0.0f;


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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
//calculate time between frames
void updateDeltaTime()
{
	float currentFrame = glfwGetTime();
	deltaTime = lastFrame==-1? 0 : (currentFrame - lastFrame)*timeScale;
	lastFrame = currentFrame;
}

int main()
{
	//create window and set up basic callbacks
	GLFWwindow* window = initWindow(windowWidth, windowHeight);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	err = fopen("logfile.txt", "w");

	//create scene instance
	Scene scene;
	initScene(&scene,window, onUpdate, onStart);
	
	scene.onStart(&scene);

	lastFrame = -1;
	while (!glfwWindowShouldClose(window))
	{
		updateDeltaTime();

		//clear the screen
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(0);
		
		scene.onUpdate(&scene, deltaTime);

		//End Draw Calls
		glfwSwapBuffers(window);
	}
	//terminate window
	glfwTerminate();
	glfwDestroyWindow(window);
	freeScene(&scene);
	return 0;
}
