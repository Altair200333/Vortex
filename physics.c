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

	//Here is the biggest part - reflection of normal component of the speed
	//1.8 is bounciness - it should be set in rigid body properties
	obj->rigidBody.lineralVel = add(obj->rigidBody.lineralVel,
		vmul(normal,
			fabs(1.8*dot(normal, obj->rigidBody.lineralVel) / sqMagnitude(normal))));
	
	float dotp = dot(obj->rigidBody.lineralVel, normal);
	float frC = dotp * dotp / sqMagnitude(obj->rigidBody.lineralVel) / sqMagnitude(normal);
	float sina;
	frC >= 1 ? (sina = 0) : (sina = sqrt(1 - frC));

	//that's the angular speed - cross product of speed and normalized offset direction
	//means the component of speed that's perpendicular to normal
	Vector3 cr = cross(obj->rigidBody.lineralVel, normalized(normal));

	//kind'a the same stuff is friction, actually it's wrong approach for friction setup, i should rely on delta time, not only speed and angles
	obj->rigidBody.lineralVel = vmul(obj->rigidBody.lineralVel, 1 - frC/10);
	
	obj->rigidBody.angluarVel = vmul(cr,-1);

	//shift to avoid being inside object
	translateGlobalV3(obj, vmul(normal, 1));
}

Vector3 collide(int ind, Object* obj[], size_t count)
{
	float floorHeight = -3;
	if (obj[ind]->position[1] < floorHeight)
	{
		collideObj(obj[ind], (Vector3) { 0, floorHeight - obj[ind]->position[1], 0 });
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
			}
		}
	}
	return (Vector3){0,0,0};
}


void computeSomething(Object* obj[], size_t count, float deltaTime)
{
	
	vec3 Force = { 0, -9.8f, 0 };
	float floorHeight = -3;

	//min possible squared angular speed
	float treshold = 0.00001;
	//printf("%f\n", 1/deltaTime);
	for (int i = 0; i < count; i++)
	{
		
		if(sqMagnitude(obj[i]->rigidBody.angluarVel)> treshold)
			rotateAxisV3(obj[i], magnitude(obj[i]->rigidBody.angluarVel)*deltaTime*100, obj[i]->rigidBody.angluarVel);
		
		Vector3 normal = collide(i, obj, count);
		
		Vector3 force = { 0,-4.8, 0 };
		force = sub(force, vmul(obj[i]->rigidBody.lineralVel,0.6));
		obj[i]->rigidBody.lineralVel = add(obj[i]->rigidBody.lineralVel, vmul(force, deltaTime/3/ obj[i]->rigidBody.mass));
		gizmosDrawLineV3(vecToVector(obj[i]->position), add(vecToVector(obj[i]->position), obj[i]->rigidBody.lineralVel));
		translateGlobalV3(obj[i], vmul(obj[i]->rigidBody.lineralVel, deltaTime));
	}

}

void addObjectToWorld(RigidBodyWorld* rw, Object* obj)
{
	if(rw->items == NULL)
	{
		rw->items = malloc(sizeof(Object*) * 100);
		if (rw->items == NULL)
		{
			printf("failed to allocate memory");
			return;
		}
		rw->capacity = 100;
		rw->size = 0;
	}
	if(rw->size == rw->capacity)
	{
		Object** newO = realloc(rw->items, sizeof(Object*) * (rw->capacity*2));
		if (newO == NULL)
		{
			printf("failed to allocate memory");
			return;
		}
		free(rw->items);
		rw->items = newO;
		rw->capacity *= 2;
	}
	rw->items[rw->size] = obj;
	rw->size++;
}
void updatePhysicsWorld(RigidBodyWorld* rw, float deltaTime)
{
	computeSomething(rw->items, rw->size, deltaTime);
}
void RigidBodyWorldInit(RigidBodyWorld* rw)
{
	rw->items = 0;
	rw->capacity = 0;
	rw->items = NULL;
}
float distanceToRay(Vector3 start, Vector3 dir, Vector3 pos)
{
	return magnitude(cross(dir, sub(pos,  start)));
}
Object* castRay(Vector3 start, Vector3 dir, RigidBodyWorld* rw)
{
	float minDst = INT_MAX;
	int minId = -1;
	for(int i=0; i < rw->size;i++)
	{
		float dst = distanceToRay(start, dir, vecToVector(rw->items[i]->position));
		if(dst< minDst && dst<0.8 && dot(dir, sub(vecToVector(rw->items[i]->position), start))>0)
		{
			minId = i;
			minDst = dst;
		}
		//printf("%f\n", dst);
	}
	return minId==-1? NULL : rw->items[minId];
}
void addObjectVel(Vector3 pos, Vector3 initVel, ListObjects* list, RigidBodyWorld* rw)
{
	appendObject(list, fromStlFile("ico1.stl"));
	translateGlobalV3(&list->objects[list->count - 1], pos);
	addObjectToWorld(rw, &(list->objects[list->count - 1]));
	for (int i = 0; i < 3; i++)
		list->objects[list->count - 1].rigidBody.lineralVel.axis[i] = initVel.axis[i];
}