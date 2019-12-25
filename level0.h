#ifndef LEVEL_0
#define LEVEL_0
#include "Scene.h"
#include "Player.h"

Player* pl;
static double posX1 = -1;
static double posY1 = -1;
static float Fspeed = 5;

void onStart(Scene* scene);
void onUpdate(Scene* scene, float dt);

#endif
