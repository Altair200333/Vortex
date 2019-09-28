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

//Екарынй бабай этого C
//Больше маллоков богу маллоков, ибо нефиг структуры терять

//Generate light source and sign it as dirrectional light
//Returns pointer to LightSource struct
LightSource* generateDirectionalLight(vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular, float strength)
{
	DirectionalLight* dr = (DirectionalLight*)malloc(sizeof(DirectionalLight));
	dr->direction[0] = direction[0];
	dr->direction[1] = direction[1];
	dr->direction[2] = direction[2];
	dr->ambient[0] = ambient[0];
	dr->ambient[1] = ambient[1];
	dr->ambient[2] = ambient[2];
	dr->diffuse[0] = diffuse[0];
	dr->diffuse[1] = diffuse[1];
	dr->diffuse[2] = diffuse[2];
	dr->specular[0] = specular[0];
	dr->specular[1] = specular[1];
	dr->specular[2] = specular[2];
	dr->strength = strength;
	
	LightSource* ls = (LightSource*)malloc(sizeof(LightSource));
	ls->type = TYPE_DIRECTIONAl_LIGHT;
	ls->lightSrc = dr;
	initLight(ls);
	return ls;
}
