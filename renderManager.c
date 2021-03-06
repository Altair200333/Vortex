#define _CRT_SECURE_NO_WARNINGS
#include "renderManager.h"
#include <string.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include "gizmos.h"

void initGLProperties()
{
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}
GLFWwindow* initWindow(int width, int height)
{
	//������������� GLFW
	glfwInit();
	//��������� GLFW
	//�������� ����������� ��������� ������ OpenGL. 
	//�������� 
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//��������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//��������� �������� ��� �������� ��������� ��������
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//���������� ����������� ��������� ������� ����
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
	initGLProperties();
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
	setVec3(standartShader, "material.diffuse",
		obj->color[0], obj->color[1], obj->color[2]);
	GLint modelLoc = glGetUniformLocation(standartShader->Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)(obj->model));

	renderVAO(obj);
}
void renderObjectCustom(Object* obj)
{
	useShader(obj->shader);
	setVec3(obj->shader, "material.diffuse", 
		obj->color[0], obj->color[1], obj->color[2]);

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

//
#define postToShader(base, id, target, type, param)\
do {\
char str[40];\
snprintf(str, 40, base##"[%d]."##target, id);	\
set##type(standartShader, str, param);\
} while (0);

//Set this light data to shader, recalculate light affect
void renderPointLight(Shader* sh, LightSource* ls) 
{
	
	//concat(sda, sda2);
	//set2(rintf);
	
	
	PointLight* pl = (PointLight*)(ls->lightSrc);
	char base[20];
	snprintf(base, 20, "pointLights[%d].", currentPointLightId); // puts string into buffer

	
	postToShader("pointLights", currentPointLightId, "position", Vec3V, (pl->position));
	postToShader("pointLights", currentPointLightId, "ambient", Vec3V, pl->ambient);
	postToShader("pointLights", currentPointLightId, "diffuse", Vec3V, pl->diffuse);
	postToShader("pointLights", currentPointLightId, "specular", Vec3V, pl->specular);
	
	postToShader("pointLights", currentPointLightId, "constant", Float, pl->constant);
	postToShader("pointLights", currentPointLightId, "linear", Float, pl->linear);
	postToShader("pointLights", currentPointLightId, "quadratic", Float, pl->quadratic);
	
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

//Proceed all lights in scene(no shadows, only "transform" params)
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

//�� ����� ������� ��������������� ��� ��������� �������, ���� ����������� � �� ������

//Recalculate lighting for single light Source
//Set shadow map/cubemap data to shader
//.. Actually renders scene from light's perspective
void recalculateShadows(Shader* s, LightSource* ls, Object** objects, int count)
{
	// configure depth map FBO
	// -----------------------
	if(ls->type != TYPE_POINT_LIGHT)
		return;
	PointLight* pl = (PointLight*)(ls->lightSrc);
	bindShadowTransform(pl);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, pl->shadowData.depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	useShader(simpleDepthShader);
	for (unsigned int i = 0; i < 6; ++i)
	{
		char base[40];
		snprintf(base, 40, "shadowMatrices[%d]", i);
		setMat4(simpleDepthShader, base, (float*)(pl->shadowData.shadowTransforms[i]));
	}
	setFloat(simpleDepthShader, "far_plane", far_plane);
	setVec3(simpleDepthShader, "lightPos", pl->position[0], pl->position[1], pl->position[2]);
	renderScene(simpleDepthShader, objects, count);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 1280, 720);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	useShader(s);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, pl->shadowData.depthCubemap);

	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderListLights(Shader* sh, LightList* list)
{
	for (int i = 0; i < list->count; i++)
	{
		list->lights[i]->render(sh, (struct LightSource*)(list->lights[i]));
	}
	setInt(sh, "pointLightsCount", currentPointLightId);
	setInt(sh, "spotLightsCount", currentSpotLightId);

	currentPointLightId = 0;
	currentSpotLightId = 0;
}

void recalculateShadowsList(Shader* s, struct LightSource* ls, ListObjects* list)
{
	recalculateShadows(s, ls, list->objects, list->count);
}
void recalculateShadowsListAtList(Shader* s, LightList* ls, ListObjects* list)
{
	for(int i=0;i<ls->count;i++	)
		recalculateShadows(s, ls->lights[i], list->objects, list->count);
}
void renderScene(Shader* shader, Object** objects, int count)
{
	for (unsigned int i = 0; i < count; i++)
	{
		renderObjectSpecificShader((objects[i]), shader);
		//rotateAxis(&(objects[i]), 0.19f, (vec3) { (i+1)%10, i%10, 0 });
	}
}
void renderListObjects(ListObjects* li)
{
	for(int i=0;i<li->count;i++)
	{
		li->objects[i]->render((li->objects[i]));
		//gizmosDrawLineV3(vecToVector(li->objects[i]->position), add(vecToVector(li->objects[i]->position), li->objects[i]->rigidBody.lineralVel));
	}
}
