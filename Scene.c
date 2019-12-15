#include "Scene.h"

void initScene(Scene* scene, void(*onUpdate)(Scene), void(*onStart)(Scene))
{
	scene->lights.lights = NULL;
	scene->lights.count = 0;
	scene->sceneObjects.count = 0;
	scene->sceneObjects.objects = NULL;
	scene->onStart = onStart;
	scene->onUpdate= onUpdate;
}
