#include "physics.h"
#include "worldObject.h"
#include <math.h>
//Every collision creates counteracting impulse applying to body
void computeCubeFall(Object* obj[], size_t count, float deltaTime)
{
	vec3 Force = { 0, -9.8f, 0 };
	float floorHeight = -3;
	for (int i = 0; i < count; i++)
	{

		

		//printf("%f %f\n", obj[i]->rotation[0] * 180 / GLM_PI, cos(obj[i]->rotation[0]));

		vec3 force = { 0, -9.8*obj[i]->rigidBody.mass, 0 };
		
		vec3 drag = {0,-0.1 * 0.5 * 1.2 * 0.47 * 1 * obj[i]->rigidBody.lineralVel[1] * obj[i]->rigidBody.lineralVel[1],0};
		glm_vec3_add(force, drag, force);

		vec3 shift = { 0,0,0 };
		float dy = obj[i]->rigidBody.lineralVel[1]* deltaTime + (0.5 * obj[i]->rigidBody.acceleration[1] * deltaTime * deltaTime);
		translateGlobal(obj[i], (vec3) { 0, dy, 0 });
		
		float new_ay = force[1] / obj[i]->rigidBody.mass;
		float avg_ay = 0.5 * (new_ay + obj[i]->rigidBody.acceleration[1]);
		obj[i]->rigidBody.lineralVel[1] += avg_ay * deltaTime;
		
		if (obj[i]->location[1]<floorHeight)
		{
			/* This is a simplification of impulse-momentum collision response. e should be a negative number, which will change the velocity's direction. */
			obj[i]->rigidBody.lineralVel[1] *= -0.5;
			/* Move the ball back a little bit so it's not still "stuck" in the wall. */
			translateGlobal(obj[i], (vec3) { 0, -obj[i]->location[1] + floorHeight, 0 });
		}

	}


} 
float sqMagintude(vec3 vec)
{
	return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
}
float collide(Object* ob, Object* obj[], size_t count)
{
	float floorHeight = -3;
	
	if(ob->location[1]<floorHeight)
	{
		return floorHeight - ob->location[1];
	}
	for (int i=0; i < count; i++)
	{
		if (ob != obj[i])
		{
			vec3 diff;
			glm_vec3_sub(ob->location, obj[i]->location, diff);
			if (sqMagintude(diff)<1)
			{
				float diff = ob->location[1] - obj[i]->location[1];
				return diff > 0 ? 1-diff: ((1+diff));
			}
		}
	}
	
	return 0;
}


void computeSomething(Object* obj[], size_t count, float deltaTime)
{
	vec3 Force = { 0, -9.8f, 0 };
	float floorHeight = -3;
	float treshold = 0.0001;
	for (int i = 0; i < count; i++)
	{
		rotateAxis(obj[i], 100*deltaTime*(1-2*i), (vec3) { 0, 1, 0 });
		float depth = collide(obj[i], obj, count);
		if(fabs(depth) > treshold)
		{
			printf("contact %f\n", depth);
			glm_vec3_scale(obj[i]->rigidBody.lineralVel, -1, obj[i]->rigidBody.lineralVel);
			translateGlobal(obj[i], (vec3){0, depth*1.0, 0});
		}
		vec3 force = { 0, -9.8*obj[i]->rigidBody.mass, 0 };

		vec3 drag = { 0,-0.1 * 0.5 * 1.2 * 0.47 * 1 * obj[i]->rigidBody.lineralVel[1] * obj[i]->rigidBody.lineralVel[1],0 };
		glm_vec3_add(force, drag, force);

		vec3 shift = { 0,0,0 };
		float dy = obj[i]->rigidBody.lineralVel[1] * deltaTime + (0.5 * obj[i]->rigidBody.acceleration[1] * deltaTime * deltaTime);
		translateGlobal(obj[i], (vec3) { 0, dy, 0 });

		float new_ay = force[1] / obj[i]->rigidBody.mass;
		float avg_ay = 0.5 * (new_ay + obj[i]->rigidBody.acceleration[1]);
		obj[i]->rigidBody.lineralVel[1] += avg_ay * deltaTime;
		//translateGlobal(obj[i], obj[i]->rigidBody.lineralVel);
	}

}