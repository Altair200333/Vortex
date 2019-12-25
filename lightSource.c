#include "lightSource.h"
#include <assert.h>

mat4 shadowProj;
mat4 tmp, tmp3;
vec3 tmp2;

void setTransform(PointLight* pl, vec3 lightPos, int id, vec3 look, vec3 look2)
{
	if (id != 0)
		glm_vec3_add(lightPos, look, tmp2);
	
	glm_lookat(lightPos, tmp2, look2, tmp);
	glm_mat4_mul(shadowProj, tmp, pl->shadowData.shadowTransforms[id]);
}
//Update cubeMap transforms
void bindShadowTransform(PointLight* pl)
{
	vec3 lightPos = { (pl->position)[0], pl->position[1], pl->position[2] };
	glm_vec3_add(lightPos,
		(vec3) {
		1.0f, 0.0f, 0.0f
	}, tmp2);
	setTransform(pl, lightPos, 0, 0, (vec3) { 0.0f, -1.0f, 0.0f });
	setTransform(pl, lightPos, 1, (vec3) { -1.0f, 0.0f, 0.0f }, (vec3) { 0.0f, -1.0f, 0.0f });
	setTransform(pl, lightPos, 2, (vec3) { 0.0f, 1.0f, 0.0f }, (vec3) { 0.0f, 0.0f, 1.0f });
	setTransform(pl, lightPos, 3, (vec3) { 0.0f, -1.0f, 0.0f }, (vec3) { 0.0f, 0.0f, -1.0f });
	setTransform(pl, lightPos, 4, (vec3) { 0.0f, 0.0f, 1.0f }, (vec3) { 0.0f, -1.0f, 0.0f });
	setTransform(pl, lightPos, 5, (vec3) { 0.0f, 0.0f, -1.0f }, (vec3) { 0.0f, -1.0f, 0.0f });
	

	
}
void initShadowProperties(LightSource* src)
{
	PointLight* pl = (PointLight*)(src->lightSrc);

	glGenFramebuffers(1, &pl->shadowData.depthMapFBO);
	// create depth cubemap texture
	glGenTextures(1, &pl->shadowData.depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, pl->shadowData.depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, pl->shadowData.depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, pl->shadowData.depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm_perspective(GLM_PI / 180.0f * 90, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane, shadowProj);

	bindShadowTransform(pl);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, pl->shadowData.depthMapFBO);
	useShader(standartShader);
	setInt(standartShader, "shadowMap", 0);
	useShader(simpleDepthShader);
	setInt(simpleDepthShader, "depthMap", 1);
}

void appendLigthSource(LightList* o, LightSource* obj)
{
	if (o->lights == NULL)
	{
		o->lights = (LightSource**)malloc(sizeof(LightSource*));
		assert(o->lights != NULL);
		o->count = 1;
		o->lights[0] = obj;
	}
	else
	{
		o->lights = (LightSource**)realloc(o->lights, sizeof(LightSource*)*(o->count + 1));
		o->lights[o->count] = obj;
		o->count++;
	}
}

void initLight(LightSource* src)
{
	if (src->type == TYPE_DIRECTIONAl_LIGHT)
	{
		src->render = renderDirectionalLight;
	}
	else if (src->type == TYPE_POINT_LIGHT)
	{
		src->render = renderPointLight;
		initShadowProperties(src);
	}
	else if (src->type == TYPE_SPOT_LIGHT)
	{
		src->render = renderSpotLight;
	}
}

void copyVec3(vec3 a, vec3 b)
{
	for(int i = 0; i < 3;i++)
	{
		a[i] = b[i];
	}
}

//Generate light source and sign it as dirrectional light
//Returns pointer to LightSource struct
LightSource* generateDirectionalLight(vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular, float strength)
{
	DirectionalLight* dr = (DirectionalLight*)malloc(sizeof(DirectionalLight));
	assert(dr != NULL);
	copyVec3(dr->direction, direction);
	copyVec3(dr->ambient, ambient);
	copyVec3(dr->diffuse, diffuse);
	copyVec3(dr->specular, specular);
	dr->strength = strength;
	
	LightSource* ls = (LightSource*)malloc(sizeof(LightSource));
	assert(ls != NULL);
	ls->type = TYPE_DIRECTIONAl_LIGHT;
	ls->lightSrc = dr;
	initLight(ls);
	return ls;
}
LightSource* generatePointLight(vec3 position, float constant, float linear,
	float quadratic, vec3 ambient, vec3 diffuse, vec3 specular)
{
	PointLight* dr = (PointLight*)malloc(sizeof(PointLight));
	assert(dr != NULL);
	copyVec3(dr->position, position);
	copyVec3(dr->ambient, ambient);
	copyVec3(dr->diffuse, diffuse);
	copyVec3(dr->specular, specular);
	
	dr->linear = linear;
	dr->quadratic = quadratic;
	dr->constant = constant;

	LightSource* ls = (LightSource*)malloc(sizeof(LightSource));
	ls->type = TYPE_POINT_LIGHT;
	ls->lightSrc = dr;

	initLight(ls);
	return ls;
}
void initLightList(LightList* ll)
{
	ll->count = 0;
	ll->lights = NULL;
}
