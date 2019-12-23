#include <stdio.h>
#include <GL/glew.h>
#include "baseShader.h"
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include "Player.h"

Player initPlayer(float fov, int w, int h)
{
	Player pl = { {	1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1}, 
					{1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 }, { 0.0f, 0.0f, 3.0f }, {0.0f, 0.0f, -1.0f}, { 0.0f, 1.0f, 0.0f }, fov, NULL };

	glm_perspective(fov, (float)w / h, 0.1f, 400.0f, pl.projection);
	glm_look((vec3){ 0.0f, 0.0f, 3.0f }, (vec3) { 0.0f, 0.0f, -1.0f }, (vec3) { 0.0f, 1.0f, 0.0f }, pl.view);

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