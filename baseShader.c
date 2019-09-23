#define _CRT_SECURE_NO_WARNINGS
#include "baseShader.h"
#include <stdio.h>


char* getShader(const char* shaderPath)
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
Shader* makeShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	GLchar* vShaderCode = getShader(vertexPath);
	printf("%s \n", vShaderCode);
	GLchar* fShaderCode = getShader(fragmentPath);
	printf("%s \n", fShaderCode);

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