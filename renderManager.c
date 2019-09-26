#include "renderManager.h"

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
void renderDirectionalLight(Shader* sh, LightSource* ls, int id) {}
void renderPointLight(Shader* sh, LightSource* ls, int id) {}
void renderSpotLight(Shader* sh, LightSource* ls, int id){}