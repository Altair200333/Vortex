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
FILE *err;
void RigidBodyWorldInit(RigidBodyWorld* rw);
void addObjectToWorld(RigidBodyWorld* rw, Object* obj);
void updatePhysicsWorld(RigidBodyWorld* rw, float deltaTime);
void computeCubeFall(Object* obj[], size_t count, float deltaTime);
void computeSomething(Object* obj[], size_t count, float deltaTime);
Object* castRayToRigidBodies(Vector3 start, Vector3 dir, RigidBodyWorld* rw);
void addObjectVel(Vector3 pos, Vector3 initVel, ListObjects* list, RigidBodyWorld* rw);

void updateCube(Object* obj, float dt);
#endif

