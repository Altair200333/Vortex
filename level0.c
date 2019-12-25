#define _CRT_SECURE_NO_WARNINGS

#include "level0.h"
#include <stdio.h>
#include "Scene.h"
#include "Player.h"
#include "gizmos.h"


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
Object* addSphereObject(vec3 pos, Scene* scene)
{
	Object* obj = appendObject(scene->sceneObjects, generateSphere());
	translateGlobal(obj, pos);
	addObjectToWorld(scene->rigidBodyWorld, obj);
	return obj;
}
//drag mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (posX1 == -1 || posX1 == -1)
	{
		posX1 = xpos;
		posY1 = ypos;
	}
	vec3 tr;
	glm_vec3_cross(pl->dir, pl->up, tr);

	glm_vec3_rotate(pl->dir, -(ypos - posY1) / 1000, tr);
	glm_vec3_rotate(pl->up, -(ypos - posY1) / 1000, tr);

	glm_vec3_rotate(pl->dir, -(xpos - posX1) / 1000, (vec3) { 0, 1, 0 });
	glm_vec3_rotate(pl->up, -(xpos - posX1) / 1000, (vec3) { 0, 1, 0 });

	posX1 = xpos;
	posY1 = ypos;
}
Object* hingle;
Object* joint;
Object* hole;
void onStart(Scene* scene)
{
	standartShader = generateShaderVertFrag("vertexSh1.vs", "fragmentSh.fs");
	lightShader = generateShaderVertFrag("lightVertexShader.vs", "lightFragmentShader.fs");

	simpleDepthShader = generateShaderVertFragGeom("point_shadows_depth.vs", "point_shadows_depth.fs", "point_shadows_depth.gs");

	glfwSetCursorPosCallback(scene->window, mouse_callback);

	vec3 lightPos = { -6.0f, 5.0f, -3.2f };

	pl = initPlayer(45, windowWidth, windowHeight);


	for (int i = 0; i < 9; i++)
	{
		appendObject(scene->sceneObjects, generateCube(1));
		translateGlobal((scene->sceneObjects->objects[i]), (vec3) { -1 + 1.2*sin(i), 2 + i * 1.1, -3 + i * 1.1*cos(i) });
	}
	
	appendObject(scene->sceneObjects, fromStlFile("engDemo.stl"));
	translateGlobalV3(scene->sceneObjects->objects[scene->sceneObjects->count - 1], (Vector3) { 1, 0, -11 });

	appendObject(scene->sceneObjects, fromStlFile("control.stl"));
	translateGlobalV3(scene->sceneObjects->objects[scene->sceneObjects->count - 1], (Vector3) { 7, 0, -7 });
	scene->sceneObjects->objects[scene->sceneObjects->count - 1]->color[1] = 1;
	
	Object* lightModels[1];
	lightModels[0] = generateCube(0.3f);
	setPos((lightModels[0]), lightPos);
	setShader((lightModels[0]), lightShader);

	appendObject(scene->sceneObjects, generatePlane(1));
	glm_translate(scene->sceneObjects->objects[scene->sceneObjects->count - 1]->model, (vec3) { 2, -4.0f, -2 });
	glm_scale(scene->sceneObjects->objects[scene->sceneObjects->count - 1]->model, (vec3) { 200.0f, 1.0f, 200.0f });
	rotateAxis(scene->sceneObjects->objects[scene->sceneObjects->count - 1], 90, (vec3) { 1.0f, 0.0f, 0.0f });

	appendObject(scene->sceneObjects, fromStlFile("cage.stl"));

	appendObject(scene->sceneObjects, fromStlFile("cage.stl"));

	LightSource* ls;
	LightSource* ps;

	ls = generateDirectionalLight((vec3) { -lightPos[0], -lightPos[1], -lightPos[2] },
		(vec3) {
		0.05f, 0.05f, 0.05f
	}, (vec3) { 0.4f, 0.4f, 0.4f }, (vec3) { 0.5f, 0.5f, 0.5f }, 0.71f);
	ps = generatePointLight((vec3) { lightPos[0], lightPos[1], lightPos[2] }, 1.0f, 0.09, 0.032, (vec3) { 0.05f, 0.05f, 0.05f }, (vec3) { 0.8f, 0.8f, 0.8f }, (vec3) { 1.0f, 1.0f, 1.0f });

	useShader(standartShader);
	setUpStandartShader();
	appendObject(scene->sceneObjects, lightModels[0]);


	initLightList(&scene->lights);
	appendLigthSource(&scene->lights, ls);
	appendLigthSource(&scene->lights, ps);

	setVec3(standartShader, "lightPos",
		((PointLight*)(ps->lightSrc))->position[0],
		((PointLight*)(ps->lightSrc))->position[1],
		((PointLight*)(ps->lightSrc))->position[2]);

	for (int i = 0; i < 10; i++)
	{
		addSphereObject((vec3) { -13 + 2 * cos(i), 1 + 1.1*i, -2 + 1.5*sin(i) }, scene);
	}
	addObjectToWorld(scene->rigidBodyWorld, scene->sceneObjects->objects[1]);

	hingle = addSphereObject((vec3) { -4, 5, -6 }, scene);
	hingle->rigidBody.isKinematic = true;
	joint = addSphereObject((vec3) { -4, 3, -6 }, scene);
	hole = addSphereObject((vec3) { -7, 3, -10 }, scene);
	hole->rigidBody.isKinematic = true;
}

void onUpdate(Scene* scene, float dt)
{
	//printf("Update\n");
	//--DRAW
	movePlayer(pl, scene->window, dt, Fspeed, scene);
	recalculateLookDirection(pl);
	setProjectionView(pl, lightShader);
	setProjectionView(pl, standartShader);
	//================= Calc shadows
	recalculateShadowsListAtList(standartShader, &scene->lights, scene->sceneObjects);
	renderListLights(standartShader, &scene->lights);
	updatePhysicsWorld(scene->rigidBodyWorld, dt);
	renderListObjects(scene->sceneObjects);
	//----

	Vector3 diff = sub(vecToVector(hingle->position), vecToVector(joint->position));
	gizmosDrawLineV3(vecToVector(hingle->position), vecToVector(joint->position));
	addForce(joint, diff, dt * 20* (magnitude(diff) - 2));

	for(int i=0;i<scene->rigidBodyWorld->size;i++)
	{
		diff = sub(vecToVector(hole->position), vecToVector(scene->rigidBodyWorld->items[i]->position));
		if(magnitude(diff)<4 && magnitude(diff)>1)
		{
			addForce(scene->rigidBodyWorld->items[i], diff, dt * 10 / (magnitude(diff)));
		}
	}
	if (pl->selection != NULL)
	{
		//gizmosDrawLineV3(vecToVector(pl.selection->position), add(vecToVector(pl.selection->position), (Vector3) { 0, 1, 0 }));
		Vector3 diff = sub(vecToVector(pl->selection->position), vecToVector(pl->eye));
		float dst = magnitude(diff);
		Vector3 ldir = vmul(normalized((vecToVector(pl->dir))), dst);

		Vector3 trgt = sub(ldir, diff);
		gizmosDrawLineV3(vecToVector(pl->selection->position), add(vecToVector(pl->selection->position), trgt));
		if (!pl->selection->rigidBody.isKinematic)
		{
			pl->selection->rigidBody.lineralVel = add(pl->selection->rigidBody.lineralVel, vmul(trgt, 3 * dt));
			pl->selection->rigidBody.lineralVel = add(pl->selection->rigidBody.lineralVel, vmul(pl->selection->rigidBody.lineralVel, -0.01));
		}
		else
		{
			translateGlobalV3(pl->selection, vmul(trgt, 0.8 * dt));
		}
	}
	//-------------//
}

