#ifndef SCENE_H
#define SCENE_H
#include "worldObject.h"
#include "lightSource.h"
#include "physics.h"


typedef struct _Scene{
	//list objects in the scene
	ListObjects sceneObjects;
	//list lights in the scene
	LightList lights;
	//list rigid bodies in the scene
	RigidBodyWorld rigidBodyWorld;

	//Update method is called every frame
	void(*onUpdate)(struct _Scene);
	//OnStart is called on scene initialization
	void(*onStart)(struct _Scene);
}Scene;

//Assign onUpdate and onStart methods to scene
void initScene(Scene* scene, void(*onUpdate)(Scene), void(*onStart)(Scene));
#endif
