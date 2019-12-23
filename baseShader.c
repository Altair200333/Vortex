#define _CRT_SECURE_NO_WARNINGS
#include "baseShader.h"
#include <stdio.h>


char* getShaderFromFile(const char* shaderPath)
{
	char * buffer = NULL;
	long length=0;
	FILE * f = fopen(shaderPath, "r");
	int counter = 0;

	if (f)
	{
		int ch;

		while ((ch = getc(f)) != EOF)
		{
			length++;
		}
		buffer = (char*)malloc(sizeof(char)*(length+1));
		fseek(f, 0, SEEK_SET);

		while ((ch = getc(f)) != EOF)
		{
			if (ch <= 127)
			{
				buffer[counter] = ch;
				counter++;
			}
		}
		fclose(f);
	}
	buffer = (char*)realloc(buffer, sizeof(char)*(counter + 1));
	buffer[counter] = '\0';
	return buffer;
}
void useShader(Shader* shader)
{
	glUseProgram(shader->Program);
}
void checkForShaderCompileErrors(GLuint shader, char* type)
{
	GLint success=1;
	GLchar infoLog[1024];
	
	glGetProgramiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader, 1024, NULL, infoLog);
		printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n",infoLog);
	}
	
}
void checkProgrammCompileErrors(GLuint shader, char* type)
{
	GLint success;
	GLchar infoLog[1024];

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader, 1024, NULL, infoLog);
		printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n", infoLog);
	}
}
Shader* generateShaderVertFrag(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	GLchar* vShaderCode = getShaderFromFile(vertexPath);
	GLchar* fShaderCode = getShaderFromFile(fragmentPath);
	//printf("%s \n", vShaderCode);
	//printf("%s \n", fShaderCode);

	//Assemble Vertex Shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	//Test succes assemble
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED %s\n", infoLog);
	}
	//Assemble Fragment(color) shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);

	//Shader programm
	Shader* shader;
	shader = (Shader*)malloc(sizeof(Shader));
	shader->Program= glCreateProgram();
	glAttachShader(shader->Program, vertexShader);
	glAttachShader(shader->Program, fragmentShader);
	glLinkProgram(shader->Program);

	glGetProgramiv(shader->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader->Program, 512, NULL, infoLog);
		printf("Error occured in assembling shaders %s\n", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	return shader;
}
Shader* generateShaderVertFragGeom(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
{
	GLchar* vShaderCode = getShaderFromFile(vertexPath);
	GLchar* fShaderCode = getShaderFromFile(fragmentPath);
	GLchar* gShaderCode = getShaderFromFile(geometryPath);
									
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkForShaderCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkForShaderCompileErrors(fragment, "FRAGMENT");
	// if geometry shader is given, compile geometry shader
	unsigned int geometry;
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);
	checkForShaderCompileErrors(geometry, "GEOMETRY");
	
	// shader Program
	Shader* shader;
	shader = (Shader*)malloc(sizeof(Shader));
	shader->Program = glCreateProgram();
	glAttachShader(shader->Program, vertex);
	glAttachShader(shader->Program, fragment);
	glAttachShader(shader->Program, geometry);
	glLinkProgram(shader->Program);
	checkProgrammCompileErrors(shader->Program, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);

	return shader;
}
//Bunch of methods for posting data to shader


void setVec3(Shader* sh, char* name, float x, float y, float z)
{
	GLint Location = glGetUniformLocation(sh->Program, name);
	
	glUniform3f(Location, x, y, z);
}
void setVec3V(Shader* sh, char* name, vec3 param)
{
	GLint Location = glGetUniformLocation(sh->Program, name);
	glUniform3f(Location, param[0], param[1], param[2]);
}
void setFloat(Shader* sh, char* name, float x)
{
	GLint Location = glGetUniformLocation(sh->Program, name);
	glUniform1f(Location, x);
}
void setInt(Shader* sh, char* name, int x)
{
	GLint Location = glGetUniformLocation(sh->Program, name);
	glUniform1i(Location, x);
}
void setVec4(Shader* sh, char* name, float x, float y, float z, float w)
{
	GLint Location = glGetUniformLocation(sh->Program, name);
	glUniform4f(Location, x, y, z, w);
}
void setMat4(Shader* sh, char* name, GLfloat* val)
{
	GLint Location = glGetUniformLocation(sh->Program, name);
	glUniformMatrix4fv(Location, 1, GL_FALSE, val);
}
