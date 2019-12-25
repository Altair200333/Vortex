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

typedef struct _Contact
{
	Vector3 contacts[4];
	int contactsCount;
	float normVel;
	Vector3 normal;
}Contact;
typedef struct _Collision
{
	Contact collisions[4];
	int collisionsCount;
}Collision;

//initialize emty collection of rigid bodies
void RigidBodyWorldInit(RigidBodyWorld* rw);
//add new object to world
void addObjectToWorld(RigidBodyWorld* rw, Object* obj);
//recalculate physics every frame
void updatePhysicsWorld(RigidBodyWorld* rw, float deltaTime);
//update all bodies in collection
void updateBodiesInWorld(Object* obj[], size_t count, float deltaTime);
//check whether ray hits  any of physical objects in scene
Object* castRayToRigidBodies(Vector3 start, Vector3 dir, RigidBodyWorld* rw);
//instantiate object with given velocity
void addObjectVel(Vector3 pos, Vector3 initVel, ListObjects* list, RigidBodyWorld* rw);
//compute cube behaviour

void updateCube(Object* obj, float dt);
void addForce(Object* obj, Vector3 force, float dt);
void resolveCollision(Object* obj, Collision contact);
Collision collideCube(Object* obj, Object* obj2, int count);
Collision collideSphere(Object* obj, Object* obj2, int count);
#endif

