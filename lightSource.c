#include "lightSource.h"


void initLight(LightSource* src)
{
	if (src->type == TYPE_DIRECTIONAl_LIGHT)
	{
		src->render = renderDirectionalLight;
	}
	else if (src->type == TYPE_POINT_LIGHT)
	{
		src->render = renderPointLight;
	}
	else if (src->type == TYPE_SPOT_LIGHT)
	{
		src->render = renderSpotLight;
	}
}