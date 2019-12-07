#include "Scene.h"

void initScene(Scene* scene, void(*onUpdate)(Scene), void(*onStart)(Scene))
{
	scene->onStart = onStart;
	scene->onUpdate= onUpdate;
}
