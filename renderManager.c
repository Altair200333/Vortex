#define _CRT_SECURE_NO_WARNINGS
#include "renderManager.h"
#include <string.h>

GLFWwindow* initWindow(int width, int height)
{
	//Инициализация GLFW
	glfwInit();
	//Настройка GLFW
	//Задается минимальная требуемая версия OpenGL. 
	//Мажорная 
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow* window = glfwCreateWindow(width, height, "Matrix..", NULL, NULL);
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

	glViewport(0, 0, width, height);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return window;
}

void renderVAO(Object* obj)
{
	glBindVertexArray(obj->VAO);
	glDrawArrays(GL_TRIANGLES, 0, obj->vCount);

	glBindVertexArray(0);
}

void renderObjectStandart(Object* obj)
{
	useShader(standartShader);

	GLint modelLoc = glGetUniformLocation(standartShader->Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)(obj->model));

	renderVAO(obj);
}
void renderObjectCustom(Object* obj)
{
	useShader(obj->shader);

	GLint modelLoc = glGetUniformLocation(obj->shader->Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)(obj->model));

	renderVAO(obj);
}
void renderObjectSpecificShader(Object* obj, Shader* sh)
{
	useShader(sh);

	GLint modelLoc = glGetUniformLocation(sh->Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)(obj->model));

	renderVAO(obj);
}

int currentPointLightId = 0;
int currentSpotLightId = 0;
void renderDirectionalLight(Shader* sh, LightSource* ls) 
{
	DirectionalLight* dr = (DirectionalLight*)(ls->lightSrc);
	setVec3(sh, "dirLight.direction", (dr->direction)[0], (dr->direction)[1], (dr->direction)[2]);
	setVec3(sh, "dirLight.ambient", (dr->ambient)[0], (dr->ambient)[1], (dr->ambient)[2]);
	setVec3(sh, "dirLight.diffuse", (dr->diffuse)[0], (dr->diffuse)[1], (dr->diffuse)[2]);
	setVec3(sh, "dirLight.specular", (dr->specular)[0], (dr->specular)[1], (dr->specular)[2]);
	setFloat(sh, "dirLight.strength", dr->strength);
}
void renderPointLight(Shader* sh, LightSource* ls) 
{
	PointLight* pl = (PointLight*)(ls->lightSrc);
	char base[20];
	snprintf(base, 20, "pointLights[%d].", currentPointLightId); // puts string into buffer
	char data[40];
	strcpy(data, base);
	strcat(data, "position");
	setVec3(standartShader, data, (pl->position)[0], pl->position[1], pl->position[2]);
	strcpy(data, base);
	strcat(data, "ambient");
	setVec3(standartShader, data, pl->ambient[0], pl->ambient[1], pl->ambient[2]);
	strcpy(data, base);
	strcat(data, "diffuse");
	setVec3(standartShader, data, pl->diffuse[0], pl->diffuse[1], pl->diffuse[2]);
	strcpy(data, base);
	strcat(data, "specular");
	setVec3(standartShader, data, pl->specular[0], pl->specular[1], pl->specular[2]);
	strcpy(data, base);
	strcat(data, "constant");
	setFloat(standartShader, data, pl->constant);
	strcpy(data, base);
	strcat(data, "linear");
	setFloat(standartShader, data, pl->linear);
	strcpy(data, base);
	strcat(data, "quadratic");
	setFloat(standartShader, data, pl->quadratic);
	currentPointLightId++;
}
void renderSpotLight(Shader* sh, LightSource* ls)
{
	SpotLight* pl = (SpotLight*)(ls->lightSrc);

	char base[30];
	snprintf(base, 20, "spotLight[%d].", currentSpotLightId); // puts string into buffer
	char data[40];
	strcpy(data, base);
	strcat(data, "position");
	setVec3(sh, data, pl->position[0], pl->position[1], pl->position[2]);
	strcpy(data, base);
	strcat(data, "direction");
	setVec3(sh, data, pl->direction[0], pl->direction[1], pl->direction[2]);
	strcpy(data, base);
	strcat(data, "ambient");
	setVec3(sh, data, pl->ambient[0], pl->ambient[1], pl->ambient[2]);
	strcpy(data, base);
	strcat(data, "diffuse");
	setVec3(sh, data, pl->diffuse[0], pl->diffuse[1], pl->diffuse[2]);
	strcpy(data, base);
	strcat(data, "specular");
	setVec3(sh, data, pl->specular[0], pl->specular[1], pl->specular[2]);
	strcpy(data, base);
	strcat(data, "constant");
	setFloat(sh, data, pl->constant);
	strcpy(data, base);
	strcat(data, "linear");
	setFloat(sh, data, pl->linear);
	strcpy(data, base);
	strcat(data, "quadratic");
	setFloat(sh, data, pl->quadratic);
	strcpy(data, base);
	strcat(data, "cutOff");
	setFloat(sh, data, pl->cutOff);
	strcpy(data, base);
	strcat(data, "outerCutOff");
	setFloat(sh, data, pl->outerCutOff);
	currentSpotLightId++;
}

void renderLights(Shader* sh, LightSource** ls, int size)
{
	for (int i = 0; i < size; i++)
	{
		(ls[i])->render(sh, ls[i]);
	}
	setInt(sh, "pointLightsCount", currentPointLightId);
	setInt(sh, "spotLightsCount", currentSpotLightId);

	currentPointLightId = 0;
	currentSpotLightId = 0;
}
