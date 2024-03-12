#pragma once
#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <iostream>
#include<fstream>
#include <string>
class Shader
{
public:
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexLocation, const char* fragmentLocation);
	std::string ReadFile(const char* file);
	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensity();
	GLuint GetAmbientColorLocation();
	void UseShader();
	void ClearShader();
	~Shader();
private:
	GLuint shaderID, uniformProjection, uniformModel=0, uniformView, uniformAmbientIntensity, uniformAmbientColor;
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

