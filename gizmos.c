#include "gizmos.h"
#include <GL/glew.h>
#include "worldObject.h"
#include "renderManager.h"
#include "vector3.h"

void gizmosDrawLine(vec3 start, vec3 end)
{
	if (!enableGizmos)
		return;
	GLfloat vert[] = {
		start[0], start[1], start[2],  0.0f,  0.0f, -1.0f,
		 end[0], end[1], end[2],  0.0f, -1.0f
	};
	Object gizmoObj = { 0, 0, {
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 }, {0,0,0},{0,0,0}, vert, 2, NULL ,
		{1.0f, 0.5f, 0.31f} };;
	glGenVertexArrays(1, &gizmoObj.VAO);
	glGenBuffers(1, &gizmoObj.VBO);

	glBindBuffer(GL_ARRAY_BUFFER, gizmoObj.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	glBindVertexArray(gizmoObj.VAO);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//use standart shader to draw green line
	useShader(standartShader);
	setVec3(standartShader, "material.diffuse",
		0, 255, 0);
	GLint modelLoc = glGetUniformLocation(standartShader->Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)(gizmoObj.model));

	glBindVertexArray(gizmoObj.VAO);
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, gizmoObj.vCount);
	glLineWidth(1);

	//free memory to prevent leaks
	glDeleteVertexArrays(1, &gizmoObj.VAO);
	glDeleteBuffers(1, &gizmoObj.VBO);
}
void gizmosDrawLineV3(Vector3 from, Vector3 to)
{
	vec3 st, end;
	VectorToVec(from, st);
	VectorToVec(to, end);
	gizmosDrawLine(st, end);
}