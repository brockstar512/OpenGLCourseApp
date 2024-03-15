#pragma once
#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <iostream>
#include<fstream>
#include <string>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "CommonValues.h"
//class DirectionalLight; //forward declaring -> this will prevent putting all the header files in here
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
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetEyeLocation();
	void SetPointLights(PointLight* pLights,unsigned int lightCount);
	void SetDirectionLight(DirectionalLight* dLight);
	void UseShader();
	void ClearShader();
	~Shader();
private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView,uniformEyePosition, uniformSpecularIntensity, uniformShininess;
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	int pointLightCount = 0;

	struct {
		GLuint uniformColor=0;
		GLuint uniformAmbientIntensity=0;
		GLuint uniformDiffuseIntensity=0;
		GLuint uniformDirection=0;

	} uniformDirectionLight;

	GLuint uniformPointLightCount = 0;
	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformDirection;
		GLuint uniformPosition;
		GLuint uniformExponent;
		GLuint uniformLinear;
		GLuint uniformConstant;


	} uniformPointLight[MAX_POINT_LIGHTS];
};

