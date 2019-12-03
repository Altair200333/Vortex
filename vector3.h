#ifndef  VECTOR3_H
#define VECTOR3_H
#include <cglm/vec3-ext.h>

typedef struct _Vector3 {
	float axis[3];
}Vector3;
Vector3 add(Vector3 a, Vector3 b);
Vector3 sub(Vector3 a, Vector3 b);

float dot(Vector3 a, Vector3 b);

Vector3 cross(Vector3 a, Vector3 b);

float sqMagnitude(Vector3 a);

float magnitude(Vector3 a);

Vector3 vdiv(Vector3 a, float val);

Vector3 vmul(Vector3 a, float val);

Vector3 normalized(Vector3 a);

Vector3 vecToVector(vec3 a);

void VectorToVec(Vector3 a, vec3 dest);

#endif
