#include  "vector3.h"
#include <cglm/vec3-ext.h>

Vector3 add(Vector3 a, Vector3 b)
{
	return (Vector3) { a.axis[0] + b.axis[0], a.axis[1] + b.axis[1], a.axis[2] + b.axis[2] };
}
Vector3 sub(Vector3 a, Vector3 b)
{
	return (Vector3) { a.axis[0] - b.axis[0], a.axis[1] - b.axis[1], a.axis[2] - b.axis[2] };
}
float dot(Vector3 a, Vector3 b)
{
	return a.axis[0] * b.axis[0] + a.axis[1] * b.axis[1] + a.axis[2] * b.axis[2];
}
Vector3 cross(Vector3 a, Vector3 b)
{
	return (Vector3) {
		a.axis[1] * b.axis[2] - b.axis[1] * a.axis[2], b.axis[0] * a.axis[2] - a.axis[0] * b.axis[2], a.axis[0] * b.axis[1] - b.axis[0] * a.axis[1]
	};
}
float sqMagnitude(Vector3 a)
{
	return dot(a, a);
}
float magnitude(Vector3 a)
{
	return sqrt(dot(a, a));
}
Vector3 vdiv(Vector3 a, float val)
{
	float tmp = 1 / val;
	return (Vector3) { a.axis[0] * tmp, a.axis[1] * tmp, a.axis[2] * tmp };
}
Vector3 vmul(Vector3 a, float val)
{
	return (Vector3) { a.axis[0] * val, a.axis[1] * val, a.axis[2] * val };
}
Vector3 normalized(Vector3 a)
{
	float invmag = 1 / magnitude(a);
	return (Vector3) { a.axis[0] * invmag, a.axis[1] * invmag, a.axis[2] * invmag };
}
Vector3 vecToVector(vec3 a)
{
	return (Vector3) { a[0], a[1], a[2] };
}
void VectorToVec(Vector3 a, vec3 dest)
{
	for (int i = 0; i < 3; i++)
		dest[i] = a.axis[i];

}