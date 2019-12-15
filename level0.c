#include "level0.h"
#include <stdio.h>
#include "Scene.h"
#include "Player.h"

Player pl2;
void initLights2(vec3 lightPos, vec3 pointLightPositions[], LightSource** ls, LightSource** ps, LightSource** ps2, LightSource** ps3)
{
	*ls = generateDirectionalLight(
		(vec3) {
		-lightPos[0], -lightPos[1], -lightPos[2]
	},
		(vec3) {
		0.05f, 0.05f, 0.05f
	}, (vec3) { 0.4f, 0.4f, 0.4f },
			(vec3) {
			0.5f, 0.5f, 0.5f
		}, 0.71f);

	*ps = generatePointLight((vec3) { lightPos[0], lightPos[1], lightPos[2] },
		1.0f, 0.09, 0.032,
		(vec3) {
		0.05f, 0.05f, 0.05f
	},
		(vec3) {
		0.8f, 0.8f, 0.8f
	},
			(vec3) {
			1.0f, 1.0f, 1.0f
		});

	*ps2 = generatePointLight((vec3) {
		pointLightPositions[1][0],
			pointLightPositions[1][1],
			pointLightPositions[1][2]
	}, 1.0f, 0.09, 0.032,
		(vec3) {
		0.05f, 0.05f, 0.05f
	},
			(vec3) {
			0.8f, 0.8f, 0.8f
		},
			(vec3) {
			1.0f, 1.0f, 1.0f
		});

	*ps3 = generatePointLight((vec3) {
		pointLightPositions[2][0],
			pointLightPositions[2][1],
			pointLightPositions[2][2]
	}, 1.0f, 0.09, 0.032,
		(vec3) {
		0.05f, 0.05f, 0.05f
	},
			(vec3) {
			0.8f, 0.9f, 0.8f
		},
			(vec3) {
			1.0f, 1.0f, 1.0f
		});
}

void onStart(Scene scene)
{
	printf("Start\n");

	vec3 lightPos = { -6.0f, 4.0f, -3.2f };

	vec3 cubePositions[] = {
		{0.0f,  0.0f,  0.0f},
		{2.0f,  5.0f, -15.0f},
		{-1.5f, -2.2f, -2.5f},
		{-3.8f, -2.0f, -12.3f},
		{2.4f, 5.0f, -3.5f},
		{-1.7f,  -1.0f, -7.5f},
		{-1.7f, 1.0f, -7.5f},
		{1.5f,  2.0f, -2.5f},
		{1.5f,  0.2f, -1.5f},
		{-1.3f,  1.0f, -1.5f}
	};

	vec3 pointLightPositions[] = {
		{lightPos[0], lightPos[1], lightPos[2]},
		{2.3f, -3.3f, -4.0f},
		{-4.0f,  6.0f, -12.0f},
	};
	pl2 = initPlayer(45, windowWidth, windowHeight);

	//glEnable(GL_CULL_FACE);

	for (unsigned int i = 0; i < 9; i++)
	{
		appendObject(&scene.sceneObjects, generateCube(1));
		translateGlobal(&(scene.sceneObjects.objects[i]), cubePositions[i]);
		//glm_translate(list.objects[i].model, cubePositions[i]);
	}

	Object lightModels[3];
	for (int i = 0; i < 3; i++)
	{
		lightModels[i] = generateCube(0.3f);
		//appendObject(&list, lightModels[i]);
		setPos(&(lightModels[i]), pointLightPositions[i]);
		//glm_scale(lightModels[i].model, (vec3) { 0.3, 0.3, 0.3 });
		setShader(&(lightModels[i]), lightShader);
	}
	RigidBodyWorldInit(&scene.rigidBodyWorld);



	appendObject(&scene.sceneObjects, generatePlane(1));
	glm_translate(scene.sceneObjects.objects[scene.sceneObjects.count - 1].model, (vec3) { 2, -4.0f, -2 });
	glm_scale(scene.sceneObjects.objects[scene.sceneObjects.count - 1].model, (vec3) { 200.0f, 1.0f, 200.0f });
	rotateAxis(&scene.sceneObjects.objects[scene.sceneObjects.count - 1], 90, (vec3) { 1.0f, 0.0f, 0.0f });

	appendObject(&scene.sceneObjects, fromStlFile("dev.stl"));
	appendObject(&scene.sceneObjects, fromStlFile("ico.stl"));
	glm_translate(scene.sceneObjects.objects[scene.sceneObjects.count - 1].model, (vec3) { -10, -2.9, -10.5 });

	appendObject(&scene.sceneObjects, fromStlFile("cage.stl"));
	setPos(&scene.sceneObjects.objects[scene.sceneObjects.count - 1], (vec3) { -5, -1, -4 });

	appendObject(&scene.sceneObjects, fromStlFile("cage.stl"));
	appendObject(&scene.sceneObjects, fromStlFile("ico1.stl"));
	translateGlobal(&scene.sceneObjects.objects[scene.sceneObjects.count - 1], (vec3) { -2.5, 3, -1.2 });
	//list.objects[list.count - 1].rigidBody.lineralVel[1] = -0.01;
	appendObject(&scene.sceneObjects, fromStlFile("ico1.stl"));
	translateGlobal(&scene.sceneObjects.objects[scene.sceneObjects.count - 1], (vec3) { -3, 0, -1 });

	appendObject(&scene.sceneObjects, fromStlFile("ico1.stl"));
	translateGlobal(&scene.sceneObjects.objects[scene.sceneObjects.count - 1], (vec3) { -3.5, 5, -1 });

	appendObject(&scene.sceneObjects, fromStlFile("ico1.stl"));
	translateGlobal(&scene.sceneObjects.objects[scene.sceneObjects.count - 1], (vec3) { -4.4, 6, -1 });

	//addSphereObject((Vector3) { -4, 7, -1 });
	addObjectVel((Vector3) { -4, 9, -1.1 }, (Vector3) { 0, 0, 1 }, &scene.sceneObjects, &scene.rigidBodyWorld);
	addObjectVel((Vector3) { -3, 9, -1.1 }, (Vector3) { 1, 0, -1 }, &scene.sceneObjects, &scene.rigidBodyWorld);

	LightSource* ls;
	LightSource* ps;
	LightSource* ps2;
	LightSource* ps3;
	initLights2(lightPos, pointLightPositions, &ls, &ps, &ps2, &ps3);


	SpotLight sli = {
	{0, 2, -4},
	{0,-1,0},
	cos(GLM_PI / 180 * 10.5f),
	cos(GLM_PI / 180 * 17.0f),
	1.0f, 0.09, 0.032,
	{0.03f, 0.03f, 0.03f},
	{0.6f, 0.6f, 0.6f},
	{1.0f, 1.0f, 1.0f} };
	LightSource sps = { TYPE_SPOT_LIGHT, &sli };
	initLight(&sps);

	LightSource* lights[] = { ls,ps, ps2, ps3 };

	useShader(standartShader);
	setFloat(standartShader, "far_plane", far_plane);
	setVec3(standartShader, "material.ambient", 1.0f, 0.5f, 0.31f);
	setVec3(standartShader, "material.specular", 0.5f, 0.5f, 0.5f);
	setFloat(standartShader, "material.shininess", 16.0f);

	ListObjects lightList = { NULL, 0 };
	appendObject(&lightList, lightModels[0]);
	appendObject(&lightList, lightModels[1]);
	appendObject(&lightList, lightModels[2]);

	LightList ll = { NULL, 0 };
	for (int i = 0; i < 4; i++)
	{
		appendLigthSource(&ll, *(lights[i]));
	}


	rotateAxis(&(scene.sceneObjects.objects[3]), 30.0, (vec3) { 0, 0, 1 });
	rotateAxis(&(scene.sceneObjects.objects[3]), 30.0, (vec3) { 1, 0, 0 });
	rotateAxis(&(scene.sceneObjects.objects[3]), 45.0, (vec3) { 0, 1, 0 });

	//rotateAxis(&(list.objects[5]), 45.0, (vec3) { 1,0, 0 });
	//rotateAxis(&(list.objects[3]), 30.0, (vec3) { 1,0, 0 });
	scene.sceneObjects.objects[6].rigidBody.lineralVel.axis[1] = -0.01;


	addObjectToWorld(&scene.rigidBodyWorld, &(scene.sceneObjects.objects[14]));
	addObjectToWorld(&scene.rigidBodyWorld, &(scene.sceneObjects.objects[15]));
	addObjectToWorld(&scene.rigidBodyWorld, &(scene.sceneObjects.objects[16]));
	addObjectToWorld(&scene.rigidBodyWorld, &(scene.sceneObjects.objects[17]));
}

void onUpdate(Scene scene)
{
	//printf("Update\n");
	
	//-------------//
}

