#pragma once
#include <GL/glew.h>
class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);
	void UseMaterial(GLuint specularInstenityLocation, GLuint shininessLocation);
	~Material();

private:
	GLfloat specularIntensity;//how bright the object is
	GLfloat shininess;//how collected the light is
};

