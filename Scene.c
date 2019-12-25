#include "Scene.h"

void initScene(Scene* scene, void(*onUpdate)(Scene), void(*onStart)(Scene))
{
	scene->lights.lights = NULL;
	scene->lights.count = 0;

	scene->onStart = onStart;
	scene->onUpdate= onUpdate;
}
