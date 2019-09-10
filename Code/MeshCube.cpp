#include "MeshCube.h"

MeshCube::MeshCube(int size) : Mesh()
{
	this->size = size;
	// TODO: fer l'escalat amb scale no amb una nova variable

	verticesSize = 24;
	GLfloat* vertices_array = new GLfloat[verticesSize]{
		0.0f * size, 0.0f * size, 0.0f * size,
		1.0f * size, 0.0f * size, 0.0f * size,
		1.0f * size, 1.0f * size, 0.0f * size,
		0.0f * size, 1.0f * size, 0.0f * size,
		0.0f * size, 0.0f * size, 1.0f * size,
		1.0f * size, 0.0f * size, 1.0f * size,
		1.0f * size, 1.0f * size, 1.0f * size,
		0.0f * size, 1.0f * size, 1.0f * size
	};

	vertices = 0u;
	glGenBuffers(1, (GLuint*) &(vertices));
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, vertices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	indicesSize = 36;
	unsigned int* indices_array = new unsigned int[indicesSize]{
		2,1,0,
		3,2,0,
		2,5,1,
		5,2,6,
		3,7,2,
		2,7,6,
		5,6,4,
		6,7,4,
		3,0,4,
		7,3,4,
		0,1,4,
		1,5,4
	};

	indices = 0u;
	glGenBuffers(1, (GLuint*) &(indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 36, indices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

MeshCube::~MeshCube()
{

}