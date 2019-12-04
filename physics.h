#ifndef  PHYSICS_H
#define PHYSICS_H
#include "worldObject.h"

//collection of all rigid bodies in scene
typedef struct _RigidBodyWorld
{
	size_t capacity;
	size_t size;
	Object** items;
}RigidBodyWorld;
void RigidBodyWorldInit(RigidBodyWorld* rw);
void addObjectToWorld(RigidBodyWorld* rw, Object* obj);
void updatePhysicsWorld(RigidBodyWorld* rw, float deltaTime);
void computeCubeFall(Object* obj[], size_t count, float deltaTime);
void computeSomething(Object* obj[], size_t count, float deltaTime);
Object* castRay(Vector3 start, Vector3 dir, RigidBodyWorld* rw);
Object* castRay(Vector3 start, Vector3 dir, RigidBodyWorld* rw);

#endif

