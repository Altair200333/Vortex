#ifndef SCENE_H
#define SCENE_H
#include "worldObject.h"
#include "lightSource.h"
#include "physics.h"


typedef struct _Scene{
	GLFWwindow* window;
	//list objects in the scene
	ListObjects* sceneObjects;
	//list lights in the scene
	LightList lights;
	//list rigid bodies in the scene
	RigidBodyWorld* rigidBodyWorld;

	//Update method is called every frame
	void(*onUpdate)(struct _Scene*, float dt);
	//OnStart is called on scene initialization
	void(*onStart)(struct _Scene*);
	//window where scene is launched
	void(*mouseCallback)(GLFWwindow* window, double xpos, double ypos);
}Scene;
//all error will be stored here
FILE *err;
//Assign window instance, onUpdate and onStart methods to scene
void initScene(Scene* scene,GLFWwindow* window, void(*onUpdate)(Scene), void(*onStart)(Scene));
void freeScene(Scene* scene);
#endif
