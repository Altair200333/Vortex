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
	
}

void onUpdate(Scene scene)
{
	//printf("Update\n");
	
	//-------------//
}

