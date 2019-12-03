#include "physics.h"
#include "worldObject.h"
#include <math.h>
#include "gizmos.h"
#include "vector3.h"
//Every collision creates counteracting impulse applying to body
void computeCubeFall(Object* obj[], size_t count, float deltaTime)
{
	vec3 Force = { 0, -9.8f, 0 };
	float floorHeight = -3;
	for (int i = 0; i < count; i++)
	{

		

		//printf("%f %f\n", obj[i]->rotation[0] * 180 / GLM_PI, cos(obj[i]->rotation[0]));

		vec3 force = { 0, -9.8*obj[i]->rigidBody.mass, 0 };
		
		vec3 drag = {0,-0.1 * 0.5 * 1.2 * 0.47 * 1 * obj[i]->rigidBody.lineralVel.axis[1] * obj[i]->rigidBody.lineralVel.axis[1],0};
		glm_vec3_add(force, drag, force);
		
		vec3 shift = { 0,0,0 };
		float dy = obj[i]->rigidBody.lineralVel.axis[1]* deltaTime + (0.5 * obj[i]->rigidBody.acceleration.axis[1] * deltaTime * deltaTime);
		translateGlobal(obj[i], (vec3) { 0, dy, 0 });
		
		float new_ay = force[1] / obj[i]->rigidBody.mass;
		float avg_ay = 0.5 * (new_ay + obj[i]->rigidBody.acceleration.axis[1]);
		obj[i]->rigidBody.lineralVel.axis[1] += avg_ay * deltaTime;
		
		if (obj[i]->position[1]<floorHeight)
		{
			/* This is a simplification of impulse-momentum collision response. e should be a negative number, which will change the velocity's direction. */
			obj[i]->rigidBody.lineralVel.axis[1] *= -0.5;
			/* Move the ball back a little bit so it's not still "stuck" in the wall. */
			translateGlobal(obj[i], (vec3) { 0, -obj[i]->position[1] + floorHeight, 0 });
		}

	}


} 


void collideObj(Object* obj, Vector3 normal)
{
	obj->rigidBody.lineralVel = add(obj->rigidBody.lineralVel,
		vmul(normal,
			fabs(1.6*dot(normal, obj->rigidBody.lineralVel) / sqMagnitude(normal))));

	translateGlobalV3(obj, vmul(normal, 1));
}

Vector3 collide(int ind, Object* obj[], size_t count)
{
	float floorHeight = -3;
	if (obj[ind]->position[1] < floorHeight)
	{
		collideObj(obj[ind], (Vector3) { 0, floorHeight - obj[ind]->position[1], 0 });
		return (Vector3) { 0, floorHeight- obj[ind]->position[1],0};
	}
	for (int i = ind; i < count; i++)
	{
		if (i != ind)
		{
			Vector3 diff = sub(vecToVector(obj[ind]->position), vecToVector(obj[i]->position));
			if (sqMagnitude(diff) <= 1)
			{
				Vector3 rad = vmul(normalized(diff), 0.5f);
				Vector3 x = sub(diff, rad);
				diff = sub(diff, vmul(x, 2));
				//gizmosDrawLineV3(vecToVector(obj[ind]->position), add(vecToVector(obj[ind]->position), diff));
				collideObj(obj[ind], diff);
				collideObj(obj[i], vmul(diff,-1));
				return diff;
			}
		}
	}
	return (Vector3){0,0,0};
}


void computeSomething(Object* obj[], size_t count, float deltaTime)
{
	
	vec3 Force = { 0, -9.8f, 0 };
	float floorHeight = -3;
	float treshold = 0.001;
	
	for (int i = 0; i < count; i++)
	{
		obj[i]->rigidBody.angluarVel.axis[1] = 0.4;
		if(sqMagnitude(obj[i]->rigidBody.angluarVel)>0.5f)
			rotateAxisV3(obj[i], magnitude(obj[i]->rigidBody.angluarVel), obj[i]->rigidBody.angluarVel);
		Vector3 normal = collide(i, obj, count);
		
		Vector3 force = { 0,-9.8, 0 };
		obj[i]->rigidBody.lineralVel = add(obj[i]->rigidBody.lineralVel, vmul(force, deltaTime/10));
		gizmosDrawLineV3(vecToVector(obj[i]->position), add(vecToVector(obj[i]->position), obj[i]->rigidBody.lineralVel));
		translateGlobalV3(obj[i], vmul(obj[i]->rigidBody.lineralVel, deltaTime));
	}

}