#include <stdio.h>
#include <GL/glew.h>
#include "baseShader.h"
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include "Player.h"
#include "GLFW/glfw3.h"
#include "physics.h"
#include "Scene.h"

void initUnitmatrix(mat4 mat)
{
	for (int i = 0; i < 16; i++)
	{
		mat[i / 4][i % 4] = i % 5 == 0 ? 1 : 0;
	}
}
void initVec3(vec3 v, vec3 ref )
{
	for (int i = 0; i < 3; i++)
		v[i] = ref[i];
}
Player* initPlayer(float fov, int w, int h)
{
	Player* pl = (Player*)malloc(sizeof(Player));
	Player p = { {	1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1}, 
					{1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 }, { 0.0f, 0.0f, 3.0f }, {0.0f, 0.0f, -1.0f}, { 0.0f, 1.0f, 0.0f }, fov, NULL };
	initUnitmatrix(pl->projection);
	initUnitmatrix(pl->view);
	initVec3(pl->eye, (vec3) { 0.0f, 0.0f, 3.0f });
	initVec3(pl->dir, (vec3) { 0.0f, 0.0f, -1.0f });
	initVec3(pl->up, (vec3) { 0.0f, 1.0f, 0.0f });
	pl->fov = fov;
	pl->selection = NULL;
	glm_perspective(fov, (float)w / h, 0.1f, 400.0f, pl->projection);
	glm_look((vec3){ 0.0f, 0.0f, 3.0f }, (vec3) { 0.0f, 0.0f, -1.0f }, (vec3) { 0.0f, 1.0f, 0.0f }, pl->view);

	return pl;
}
void recalculateLookDirection(Player* pl)
{
	glm_look(pl->eye, pl->dir, pl->up, pl->view);

}
void translatePlayer(Player* pl, vec3 dir)
{
	pl->eye[0] += dir[0];
	pl->eye[1] += dir[1];
	pl->eye[2] += dir[2];
}
void translateMagnitude(Player* pl, vec3 dir, float m)
{
	pl->eye[0] += dir[0]*m;
	pl->eye[1] += dir[1]*m;
	pl->eye[2] += dir[2]*m;
}
void setProjectionView(Player* pl, Shader* s)
{
	useShader(s);

	setVec3(s, "viewPos", pl->eye[0], pl->eye[1], pl->eye[2]);
	GLint viewLoc = glGetUniformLocation(s->Program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)pl->view);

	GLint projectionLoc = glGetUniformLocation(s->Program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)pl->projection);
}

bool down = false, Rdown = false;
void movePlayer(Player* pl,  GLFWwindow* window, float deltaTime, float Fspeed, Scene* scene)
{
	float speed = Fspeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		translateMagnitude(pl, pl->dir, speed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		translateMagnitude(pl, pl->dir, -speed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		vec3 tr;
		glm_vec3_cross(pl->dir, pl->up, tr);
		translateMagnitude(pl, tr, speed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		vec3 tr;
		glm_vec3_cross(pl->dir, pl->up, tr);
		translateMagnitude(pl, tr, -speed);

	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		translateMagnitude(pl, pl->up, speed);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		translateMagnitude(pl, pl->up, -speed);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !down)
	{
		down = true;
		printf("Down\n");

		pl->selection = castRayToRigidBodies(vecToVector(pl->eye), vecToVector(pl->dir), scene->rigidBodyWorld);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && down)
	{
		down = false;
		//printf("B\n");
		pl->selection = NULL;

	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !Rdown)
	{
		Rdown = true;
		Object* object = generateSphere();
		translateGlobalV3(object, add(vecToVector(pl->eye), vmul(vecToVector(pl->dir), 1.5f)));
		appendObject(scene->sceneObjects, object);
		addObjectToWorld(scene->rigidBodyWorld, object);
		object->rigidBody.lineralVel = vmul(vecToVector(pl->dir), 8.5f);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && Rdown)
	{
		Rdown = false;
		//printf("B\n");
	}
}