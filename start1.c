#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "baseShader.h"
#include "worldObject.h"
#include "renderManager.h"
#include "Player.h"
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>


bool EdgeViewMode = false;
Player pl;
double posX1=-1, posY1=-1;
float speed = 0.05f;
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
	if (key == GLFW_KEY_W && action != GLFW_RELEASE)
	{
		translateM(&pl, pl.dir, speed);
	}
	if (key == GLFW_KEY_S && action != GLFW_RELEASE)
	{
		translateM(&pl, pl.dir, -speed);
	}
	if (key == GLFW_KEY_D && action != GLFW_RELEASE)
	{
		vec3 tr;
		glm_vec3_cross(pl.dir, pl.up, tr);
		translateM(&pl, tr, speed);
	}
	if (key == GLFW_KEY_A && action != GLFW_RELEASE)
	{
		vec3 tr;
		glm_vec3_cross(pl.dir, pl.up, tr);
		translateM(&pl, tr, -speed);
	}
	if (key == GLFW_KEY_E && action != GLFW_RELEASE)
	{
		translateM(&pl, pl.up, speed);
	}
	if (key == GLFW_KEY_Q && action != GLFW_RELEASE)
	{
		translateM(&pl, pl.up, -speed);
	}	
}
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

int main()
{
	//Инициализация GLFW
	glfwInit();
	//Настройка GLFW
	//Задается минимальная требуемая версия OpenGL. 
	//Мажорная 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow* window = glfwCreateWindow(1000, 600, "Matrix..", NULL, NULL);
	if (window == NULL)
	{	
		printf("Failed to create GLFW window \n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW\n");
		return -1;
	}
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	unsigned long counter = 1;
	
	GLfloat vertices[] = {
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

	
	GLuint VBO, containerVAO;
	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	glEnable(GL_DEPTH_TEST);
	Shader* s;
	s = makeShader("vertexSh1.vs", "fragmentSh.fs");
	Shader* lightShader;
	lightShader = makeShader("lightVertexShader.vs", "lightFragmentShader.fs");

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
	pl = initPlayer(44.9, width, height);

	//glEnable(GL_CULL_FACE);
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//--DRAW
		
		recalculate(&pl);

		glBindVertexArray(containerVAO);
		useShader(s);
		setProjectionView(&pl, s);

		setVec3(s, "lightColor", 1, 1, 1);
		setVec3(s, "light.position", 0, 5, -5);
		setVec3(s, "viewPos", pl.eye[0], pl.eye[1], pl.eye[2]);

		setVec3(s, "light.ambient", 0.2f, 0.2f, 0.2f);
		setVec3(s, "light.diffuse", 0.6f, 0.6f, 0.6f); // darken the light a bit to fit the scene
		setVec3(s, "light.specular", 1.0f, 1.0f, 1.0f);
		//setVec3(s, "light.direction", 1.0f, -1.0f, 1.0f);
		setInt(s,  "light.type", 2);

		//printf("%f %f %f \n", pl.dir[0], pl.dir[1], pl.dir[2]);
		setVec3(s, "light.position", pl.eye[0], pl.eye[1], pl.eye[2]);
		setVec3(s, "light.direction", pl.dir[0], pl.dir[1], pl.dir[2]);
		setFloat(s,"light.cutOff", cos(3.14/180*12.5f));
		setFloat(s,"light.outerCutOff", cos(3.14/180*19.5f));

		setFloat(s,"light.constant", 1.0f);
		setFloat(s,"light.linear", 0.09f);
		setFloat(s,"light.quadratic", 0.032f);

		setVec3(s, "material.ambient", 1.0f, 0.5f, 0.31f);
		setVec3(s, "material.diffuse", 1.0f, 0.5f, 0.31f);
		setVec3(s, "material.specular", 0.5f, 0.5f, 0.5f);
		setFloat(s,"material.shininess", 32.0f);

		for (unsigned int i = 0; i < 10; i++)
		{
			
			mat4 model = {
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
			};
			
			glm_translate(model, cubePositions[i]);

			//glm_rotate(model, 3.1415926 / 180 * (float)counter/100, (vec3) { 1.0f-0.1f*i, 0.0f+(float)i*0.1f, 0.0f });
			float angle = 20.0f * i;
			GLint modelLoc = glGetUniformLocation(s->Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		useShader(lightShader);
		setProjectionView(&pl, lightShader);
		mat4 LightModel = {
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
		};
		glm_translate(LightModel, (vec3) {0, 5, -5});
		glm_scale(LightModel, (vec3) { 0.3f, 0.3f, 0.3f });
		GLint modelLoc = glGetUniformLocation(lightShader->Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)(LightModel));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		counter++;
		//End Draw Calls
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &containerVAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}