#include "physics.h"
#include "worldObject.h"
//Every collision creates counteracting impulse applying to body
void computeCubeFall(Object* obj[], size_t count, float deltaTime)
{
	vec3 Force = { 0, -9.8f, 0 };
	float floorHeight = -3;
	for (int i = 0; i < count; i++)
	{

		

		//printf("%f %f\n", obj[i]->rotation[0] * 180 / GLM_PI, cos(obj[i]->rotation[0]));

		vec3 force = { 0, -9.8, 0 };
		glm_vec3_add(force, (vec3) {0, -1 * 0.5 * 1.2 * 0.47 * 1 * obj[i]->rigidBody.lineralVel[1] * obj[i]->rigidBody.lineralVel[1], 0}, force);

		/* Verlet integration for the y-direction */
		float dy = obj[i]->rigidBody.lineralVel[1]* deltaTime + (0.5 * obj[i]->rigidBody.acceleration[1] * deltaTime * deltaTime);
		/* The following line is because the math assumes meters but we're assuming 1 cm per pixel, so we need to scale the results */
		translateGlobal(obj[i], (vec3) { 0, dy, 0 });
		
		float new_ay = force[1] / 1;
		float avg_ay = 0.5 * (new_ay + obj[i]->rigidBody.acceleration[1]);
		obj[i]->rigidBody.lineralVel[1] += avg_ay * deltaTime;
		
		
		//(obj[i])->rigidBody.lineralVel[1] -= 9.81*deltaTime / 4;
		//vec3 shift = { 0,obj[i]->rigidBody.lineralVel[1] * deltaTime,0 };
		//translateGlobal(obj[i], shift);
		if (obj[i]->location[1]<floorHeight)
		{
			/* This is a simplification of impulse-momentum collision response. e should be a negative number, which will change the velocity's direction. */
			obj[i]->rigidBody.lineralVel[1] *= -0.5;
			/* Move the ball back a little bit so it's not still "stuck" in the wall. */
			translateGlobal(obj[i], (vec3) { 0, -obj[i]->location[1] + floorHeight, 0 });
		}

	}


}