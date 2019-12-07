#ifndef SCENE_H
#define SCENE_H
#include "worldObject.h"
#include "lightSource.h"
#include "physics.h"

typedef struct _Scene{
	ListObjects sceneObjects;
	LightList lights;
	RigidBodyWorld rigidBodyWorld;
	void(*onUpdate)(struct _Scene);
	void(*onStart)(struct _Scene);
}Scene;

void initScene(Scene* scene, void(*onUpdate)(Scene), void(*onStart)(Scene));
#endif
