#pragma once
#ifndef textType_h
#define textType_h
#include <stdio.h>
#include <GL/glew.h>; 
#include <ft2build.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include "baseShader.h"
#include FT_FREETYPE_H

FT_Face face;
FT_Library ft;

GLuint TextVBO, textVAO;
void RenderText(Shader* shader, char* text, GLfloat x, GLfloat y, GLfloat scale, vec3 color);

typedef struct  {
	GLuint TextureID;   // ID handle of the glyph texture
	vec2 Size;    // Size of glyph
	vec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
}Character;
Character Characters[128];

#endif