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
#include "textType.h"
#include <ft2build.h>
#include FT_FREETYPE_H

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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
	

	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
	}
	if (FT_New_Face(ft, "Acme-Regular.ttf", 0, &face)) {
		fprintf(stderr, "Could not open font\n");
	}
	FT_Set_Pixel_Sizes(face, 0, 48);

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	Shader* s;
	s = makeShader("vertexSh1.vs", "fragmentSh.fs");
	Shader* lightShader;
	lightShader = makeShader("lightVertexShader.vs", "lightFragmentShader.fs");

	
	//FT_New_Face(ft, "fonts/arial.ttf", 0, &face);

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
		{1.0f,  2.0f,  -5.0f},
		{2.3f, -3.3f, -4.0f},
		{-4.0f,  2.0f, -12.0f},
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

		setInt(s, "pointLightsCount", 2);
		setInt(s, "spotLightsCount", 1);

		setVec3(s, "viewPos", pl.eye[0], pl.eye[1], pl.eye[2]);
		// directional light
		setVec3(s, "dirLight.direction", -0.2f, -1.0f, -0.3f);
		setVec3(s, "dirLight.ambient", 0.05f, 0.05f, 0.05f);
		setVec3(s, "dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		setVec3(s, "dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		setVec3(s,"pointLights[0].position", pointLightPositions[0][0], pointLightPositions[0][1], pointLightPositions[0][2]);
		setVec3(s,"pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		setVec3(s,"pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		setVec3(s,"pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		setFloat(s,"pointLights[0].constant", 1.0f);
		setFloat(s,"pointLights[0].linear", 0.09);
		setFloat(s,"pointLights[0].quadratic", 0.032);
		// point light 2
		setVec3(s, "pointLights[1].position", pointLightPositions[1][0], pointLightPositions[1][1], pointLightPositions[1][2]);
		setVec3(s, "pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		setVec3(s, "pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		setVec3(s, "pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		setFloat(s, "pointLights[1].constant", 1.0f);
		setFloat(s, "pointLights[1].linear", 0.09);
		setFloat(s, "pointLights[1].quadratic", 0.032);
		// point light 3
		
		setVec3(s, "pointLights[2].position", pointLightPositions[2][0], pointLightPositions[2][1], pointLightPositions[2][2]);
		setVec3(s, "pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		setVec3(s, "pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		setVec3(s, "pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		setFloat(s, "pointLights[2].constant", 1.0f);
		setFloat(s, "pointLights[2].linear", 0.09);
		setFloat(s, "pointLights[2].quadratic", 0.032);
		
		// spotLight
		setVec3(s, "spotLight[0].position", pl.eye[0], pl.eye[1], pl.eye[2]);
		setVec3(s, "spotLight[0].direction", pl.dir[0], pl.dir[1], pl.dir[2]);
		setVec3(s, "spotLight[0].ambient", 0.03f, 0.03f, 0.03f);
		setVec3(s, "spotLight[0].diffuse", 0.6f, 0.6f, 0.6f);
		setVec3(s, "spotLight[0].specular", 1.0f, 1.0f, 1.0f);
		setFloat(s,"spotLight[0].constant", 1.0f);
		setFloat(s,"spotLight[0].linear", 0.09);
		setFloat(s,"spotLight[0].quadratic", 0.032);
		setFloat(s,"spotLight[0].cutOff", cos(GLM_PI/180 * 10.5f));
		setFloat(s,"spotLight[0].outerCutOff", cos(GLM_PI / 180 * 17.0f));

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
		for (unsigned int i = 0; i < 3; i++)
		{
			mat4 LightModel = {
						1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1
			};
			glm_translate(LightModel, pointLightPositions[i]);
			glm_scale(LightModel, (vec3) { 0.3f, 0.3f, 0.3f });
			GLint modelLoc = glGetUniformLocation(lightShader->Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)(LightModel));

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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