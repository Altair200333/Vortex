#define _CRT_SECURE_NO_WARNINGS
#include "physics.h"
#include "worldObject.h"
#include <math.h>
#include "gizmos.h"
#include "vector3.h"
#include <assert.h>

//Every collision creates counteracting impulse applying to body
void resolveContact(Object* obj, Vector3 middle, int collisionCount, Vector3 normal, float normVel2);
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
			fabs(1.4*dot(normal, obj->rigidBody.lineralVel) / sqMagnitude(normal))));
	
	float dotp = dot(obj->rigidBody.lineralVel, normal);
	float frC = dotp * dotp / sqMagnitude(obj->rigidBody.lineralVel) / sqMagnitude(normal);
	if (isnan(frC))
		return;
	float sina;
	frC >= 1 ? (sina = 0) : (sina = sqrt(1 - frC));

	//that's the angular speed - cross product of speed and normalized offset direction
	//means the component of speed that's perpendicular to normal
	Vector3 cr = cross(obj->rigidBody.lineralVel, normalized(normal));
		
	obj->rigidBody.lineralVel = vmul(obj->rigidBody.lineralVel, 1 - frC/10);
	//printf("%g \n", frC);
	
	obj->rigidBody.angluarVel = vmul(cr,-1);

	//shift to avoid being inside object
	translateGlobalV3(obj, vmul(normal, 1));
}

Vector3 collide(int ind, Object* obj[], size_t count)
{
	Collision col = obj[ind]->rigidBody.findContact(obj[ind], NULL, 0);
	resolveCollision(obj[ind], col);

	for (int i = ind; i < count; i++)
	{
		if (i != ind)
		{
			
			col = obj[i]->rigidBody.findContact(obj[i], obj[ind], 1);
			Collision col2 = obj[ind]->rigidBody.findContact(obj[ind], obj[i], 1);
			resolveCollision(obj[i], col);
			resolveCollision(obj[ind], col2);
		}
	}
	return (Vector3){0,0,0};
}

void applyGravity(Object* obj, float deltaTime)
{
	Vector3 force = { 0,-9.8, 0 };
	Vector3 vv = vmul(obj->rigidBody.lineralVel, 0.6);
	force = sub(force, vv);

	obj->rigidBody.lineralVel = add(obj->rigidBody.lineralVel, vmul(force, deltaTime*0.3f / obj->rigidBody.mass));
}

void updateBodiesInWorld(Object* obj[], size_t count, float deltaTime)
{
	
	vec3 Force = { 0, -9.8f, 0 };
	float floorHeight = -3;

	//min possible squared angular speed
	float treshold = 0.00001;
	//printf("%f\n", 1/deltaTime);
	for (int i = 0; i < count; i++)
	{
		if (obj[i]->rigidBody.type != TYPE_CUBE)
		{
			Vector3 normal = collide(i, obj, count);
		}
		if (obj[i]->rigidBody.type == TYPE_CUBE)
		{
			Collision col = obj[i]->rigidBody.findContact(obj[i], NULL, 0);
			resolveCollision(obj[i], col);
		}
		if (!obj[i]->rigidBody.isKinematic)
		{
			applyGravity(obj[i], deltaTime);
			translateGlobalV3(obj[i], vmul(obj[i]->rigidBody.lineralVel, deltaTime));
			if (sqMagnitude(obj[i]->rigidBody.angluarVel) > treshold)
				rotateAxisV3(obj[i], magnitude(obj[i]->rigidBody.angluarVel)*deltaTime * obj[i]->rigidBody.Jmul, obj[i]->rigidBody.angluarVel);
		}
	}

}

void addObjectToWorld(RigidBodyWorld* rw, Object* obj)
{
	if(rw->items == NULL)
	{
		rw->items = malloc(sizeof(Object*) * 20);
		if (rw->items == NULL)
		{
			printf("failed to allocate memory");
			return;
		}
		rw->capacity = 20;
		rw->size = 0;
	}
	if(rw->size == rw->capacity)
	{
		rw->items = (Object**)realloc(rw->items, sizeof(Object*) * (rw->capacity*2));
		if (rw->items == NULL)
		{
			printf("failed to allocate memory");
			return;
		}
		
		rw->capacity *= 2;
	}
	rw->items[rw->size] = obj;
	rw->size++;
}
void updatePhysicsWorld(RigidBodyWorld* rw, float deltaTime)
{
	updateBodiesInWorld(rw->items, rw->size, deltaTime);
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
Object* castRayToRigidBodies(Vector3 start, Vector3 dir, RigidBodyWorld* rw)
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

vec3 ver[] = { {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5},{0.5, -0.5, 0.5}, {-0.5, 0.5, -0.5} };

void resolveCollision(Object* obj, Collision collision)
{
	for (int k = 0; k < collision.collisionsCount; k++)
	{

		Vector3 middle = { 0,0,0 };
		for (int j = 0; j < collision.collisions[k].contactsCount; j++)
		{
			middle = add(collision.collisions[k].contacts[j], middle);
		}
		resolveContact(obj, middle, collision.collisions[k].contactsCount, collision.collisions[k].normal, collision.collisions[k].normVel);
	}
}
void resolveContact(Object* obj, Vector3 middle, int collisionCount, Vector3 normal, float normVel2)
{
	float cosa = dot(obj->rigidBody.lineralVel, normalized(normal)) / magnitude(obj->rigidBody.lineralVel);
	Vector3 normalVelocity = vmul(normalized(normal), cosa*magnitude(obj->rigidBody.lineralVel));
	if (obj->rigidBody.isKinematic)
	{
		return;
	}
	if (collisionCount > 0)
	{
		if (sqMagnitude(normal) > 0.25)
			normal = vmul(normalized(normal), 0.25);
		translateGlobalV3(obj, normal);
	}
	if (collisionCount > 0 && collisionCount < 3)
	{
		middle = vmul(middle, 1.0f / collisionCount);
		//gizmosDrawLineV3(middle, vecToVector(obj->position));
		Vector3 g = { 0, -4,0 };
		Vector3 rot = cross((Vector3) {0,g.axis[1]+obj->rigidBody.lineralVel.axis[1]*10,0},
				sub(middle, vecToVector(obj->position)));
		//gizmosDrawLineV3(middle, add(middle, rot));
		
		obj->rigidBody.angluarVel = add(vmul(obj->rigidBody.angluarVel, 0.6), rot);
		//rotateAroundAxisV3(obj, dt*sqMagnitude(rot), rot, middle);

	}
	if (collisionCount > 0)
	{
		float k = obj->rigidBody.bounciness;
		Vector3 normalVelocity = vmul(normalized(normal), -k * cosa*magnitude(obj->rigidBody.lineralVel));
		Vector3 tanVelocity = sub(obj->rigidBody.lineralVel, vmul(normalVelocity, 1.0f / k));
		
		gizmosDrawLineV3(vecToVector(obj->position), add(vecToVector(obj->position), normalVelocity));
		
		Vector3 tanXg = cross(tanVelocity, vmul(normalized(normal), -obj->rigidBody.J));
		obj->rigidBody.angluarVel =  add(tanXg, vmul(obj->rigidBody.angluarVel, obj->rigidBody.angularSaver));

		if (dot(obj->rigidBody.lineralVel, normal) < 0)
		{
			obj->rigidBody.lineralVel = add(obj->rigidBody.lineralVel, normalVelocity);
		}
		obj->rigidBody.lineralVel = add(obj->rigidBody.lineralVel, vmul(normalized(normal), fabs(normVel2)/5));
		obj->rigidBody.lineralVel = sub(obj->rigidBody.lineralVel, vmul(tanVelocity, 0.02));

		Vector3 tanAngularVelocity = cross(normalized(normal), obj->rigidBody.angluarVel);
		//gizmosDrawLineV3(vecToVector(obj->position), add(vecToVector(obj->position), tanAngularVelocity));
		obj->rigidBody.lineralVel = sub(obj->rigidBody.lineralVel, vmul(tanAngularVelocity, obj->rigidBody.angularFriction));
	}
}

void addForce(Object* obj, Vector3 force, float dt)
{
	if(!obj->rigidBody.isKinematic)
		obj->rigidBody.lineralVel = add(obj->rigidBody.lineralVel, vmul(force, dt));
}
Collision collideCube(Object* obj, Object* obj2, int count)
{
	Collision col;
	col.collisionsCount = 0;
	

	vec3 localVertexPos;
	vec3 vertexPos;
	col.collisions[col.collisionsCount].contactsCount = 0;
	for (int i = 0; i < 8; i++)
	{
		glm_mat4_mulv3(obj->model, ver[i], 0, localVertexPos);
		glm_vec3_add(obj->position, localVertexPos, vertexPos);
		if (vertexPos[1] < -3.5f && col.collisions[col.collisionsCount].contactsCount < 4)
		{
			col.collisions[col.collisionsCount].contacts[col.collisions[col.collisionsCount].contactsCount] = vecToVector(vertexPos);
			col.collisions[col.collisionsCount].contactsCount++;		
		}
	}
	if (col.collisions[0].contactsCount > 0)
	{
		float tr = -3.5f - col.collisions[0].contacts[0].axis[1];
		col.collisions[0].normal = (Vector3) { 0, tr, 0 };
		col.collisionsCount++;
		col.collisions[0].normVel = 0;
	}
	return col;
}
Collision collideSphere(Object* obj, Object* obj2, int count)
{
	Collision col;
	col.collisionsCount = 0;
	float floorHeight = -3;
	if (count == 0)
	{
		if (obj->position[1] < floorHeight)
		{
			
			col.collisions[col.collisionsCount].normal = (Vector3) { 0, floorHeight - obj->position[1], 0 };
			col.collisions[col.collisionsCount].contacts[0] = (Vector3) { obj->position[0], obj->position[1] - 0.5, obj->position[2] };
			col.collisions[col.collisionsCount].contactsCount = 1;
			col.collisions[col.collisionsCount].normVel = 0;
			col.collisionsCount++;
		}
		return col;
	}
	Vector3 diff = sub(vecToVector(obj2->position), vecToVector(obj->position));
	if (sqMagnitude(diff) <= 1 && obj->rigidBody.type != TYPE_CUBE)
	{
		Vector3 rad = vmul(normalized(diff), 0.5f);
		Vector3 x = sub(diff, rad);
		diff = sub(diff, vmul(x, 2));
		//gizmosDrawLineV3(vecToVector(obj[ind]->position), add(vecToVector(obj[ind]->position), diff));

		float vel = magnitude(obj2->rigidBody.lineralVel);
		float cosa = vel > 0 ? dot(obj2->rigidBody.lineralVel, normalized(vmul(diff, -1))) / magnitude(obj2->rigidBody.lineralVel) : 0;

		col.collisions[col.collisionsCount].normal = vmul(diff, -1);
		col.collisions[col.collisionsCount].contacts[0] = add(vecToVector(obj->position), vmul(rad, 1));
		col.collisions[col.collisionsCount].contactsCount=1;
		col.collisions[col.collisionsCount].normVel = cosa * magnitude(obj2->rigidBody.lineralVel);
		col.collisionsCount++;

	}
	return col;

}