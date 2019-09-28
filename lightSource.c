#include "lightSource.h"

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

	mat4 shadowProj;
	glm_perspective(GLM_PI / 180.0f * 90, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane, shadowProj);
	mat4 tmp, tmp3;
	vec3 tmp2;
	vec3 lightPos = { (pl->position)[0], pl->position[1], pl->position[2] };
	glm_vec3_add(lightPos,
		(vec3) {
		1.0f, 0.0f, 0.0f
	}, tmp2);
	glm_lookat(lightPos, tmp2, (vec3) { 0.0f, -1.0f, 0.0f }, tmp);
	glm_mat4_mul(shadowProj, tmp, pl->shadowData.shadowTransforms[0]);

	glm_vec3_add(lightPos, (vec3) { -1.0f, 0.0f, 0.0f }, tmp2);
	glm_lookat(lightPos, tmp2, (vec3) { 0.0f, -1.0f, 0.0f }, tmp);
	glm_mat4_mul(shadowProj, tmp, pl->shadowData.shadowTransforms[1]);

	glm_vec3_add(lightPos, (vec3) { 0.0f, 1.0f, 0.0f }, tmp2);
	glm_lookat(lightPos, tmp2, (vec3) { 0.0f, 0.0f, 1.0f }, tmp);
	glm_mat4_mul(shadowProj, tmp, pl->shadowData.shadowTransforms[2]);

	glm_vec3_add(lightPos, (vec3) { 0.0f, -1.0f, 0.0f }, tmp2);
	glm_lookat(lightPos, tmp2, (vec3) { 0.0f, 0.0f, -1.0f }, tmp);
	glm_mat4_mul(shadowProj, tmp, pl->shadowData.shadowTransforms[3]);

	glm_vec3_add(lightPos, (vec3) { 0.0f, 0.0f, 1.0f }, tmp2);
	glm_lookat(lightPos, tmp2, (vec3) { 0.0f, -1.0f, 0.0f }, tmp);
	glm_mat4_mul(shadowProj, tmp, pl->shadowData.shadowTransforms[4]);

	glm_vec3_add(lightPos, (vec3) { 0.0f, 0.0f, -1.0f }, tmp2);
	glm_lookat(lightPos, tmp2, (vec3) { 0.0f, -1.0f, 0.0f }, tmp);
	glm_mat4_mul(shadowProj, tmp, pl->shadowData.shadowTransforms[5]);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, pl->shadowData.depthMapFBO);
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
LightSource* generatePointLight(vec3 position, float constant, float linear,
	float quadratic, vec3 ambient, vec3 diffuse, vec3 specular)
{
	PointLight* dr = (PointLight*)malloc(sizeof(PointLight));

	dr->position[0] = position[0];
	dr->position[1] = position[1];
	dr->position[2] = position[2];
	dr->ambient[0] = ambient[0];
	dr->ambient[1] = ambient[1];
	dr->ambient[2] = ambient[2];
	dr->diffuse[0] = diffuse[0];
	dr->diffuse[1] = diffuse[1];
	dr->diffuse[2] = diffuse[2];
	dr->specular[0] = specular[0];
	dr->specular[1] = specular[1];
	dr->specular[2] = specular[2];
	dr->linear = linear;
	dr->quadratic = quadratic;
	dr->constant = constant;

	LightSource* ls = (LightSource*)malloc(sizeof(LightSource));
	ls->type = TYPE_POINT_LIGHT;
	ls->lightSrc = dr;

	initLight(ls);
	return ls;
}