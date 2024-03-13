#pragma once
#include <GL/glew.h>
class Mesh
{
public:
	void CreateMesh(GLfloat* vertices, unsigned* indeces, unsigned numVertices, unsigned int numOfIndecies);
	void RenderMesh();
	void ClearMesh();
	~Mesh();
private:
	GLuint VAO, VBO, IBO = 0;
	GLsizei indexCount = 0;
	const int vertextDataPointsPerEntry = 8;
};

