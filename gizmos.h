#ifndef  GIZMOS_H
#define GIZMOS_H
#include <cglm/vec3-ext.h>
#include "vector3.h"

static bool enableGizmos = true;
/*Draws a green line between start and end points, use for debugging*/
void gizmosDrawLine(vec3 from, vec3 to);
void gizmosDrawLineV3(Vector3 from, Vector3 to);

#endif