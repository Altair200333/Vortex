#ifndef SCENE_H
#define SCENE_H
#include "worldObject.h"
#include "lightSource.h"

typedef struct _Scene{
	ListObjects sceneObjects;
	LightList lights;
}Scene;

void procceedScene(Scene* scene);
#endif
