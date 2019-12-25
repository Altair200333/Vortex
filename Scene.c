#include "Scene.h"
#include <assert.h>

void initScene(Scene* scene, GLFWwindow* window, void(*onUpdate)(Scene), void(*onStart)(Scene))
{
	scene->window = window;
	scene->lights.lights = NULL;
	scene->lights.count = 0;

	scene->onStart = onStart;
	scene->onUpdate= onUpdate;
	
	scene->sceneObjects = (ListObjects*)malloc(sizeof(ListObjects));
	assert(scene->sceneObjects != NULL);
	scene->sceneObjects->count = 0;
	scene->sceneObjects->objects = 0;
	
	scene->rigidBodyWorld = (RigidBodyWorld*)malloc(sizeof(RigidBodyWorld));
	assert(scene->rigidBodyWorld != NULL);
	RigidBodyWorldInit(scene->rigidBodyWorld);
}
void freeScene(Scene* scene)
{
	for(int i=0; i < scene->sceneObjects->count;i++)
	{
		if (scene->sceneObjects->objects[i]->wasAllocated)
			free(scene->sceneObjects->objects[i]->vertices);
		free(scene->sceneObjects->objects[i]);
	}
	free(scene->sceneObjects);
	free(scene->rigidBodyWorld);
}