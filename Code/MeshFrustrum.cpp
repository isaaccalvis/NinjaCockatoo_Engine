#include "MeshFrustrum.h"

MeshFrustrum::MeshFrustrum() : Mesh()
{
	verticesSize = 24;
	verticesArray = new GLfloat[verticesSize]{
		0.0f,0.0f, 0.0f,
		1.0f,0.0f, 0.0f,
		1.0f,0.0f, 1.0f,
		0.0f,0.0f, 1.0f,
		0.25f, 1.0f, 0.25f,
		0.75f, 1.0f, 0.25f,
		0.75f, 1.0f, 0.75f,
		0.25f, 1.0f, 0.75f
	};

	vertices = 0u;
	glGenBuffers(1, (GLuint*) &(vertices));
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesSize, verticesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	indicesSize = 36;
	unsigned int* indicesArray = new unsigned int[indicesSize] {
		2, 1, 0,
			3, 2, 0,
			2, 5, 1,
			5, 2, 6,
			3, 7, 2,
			2, 7, 6,
			5, 6, 4,
			6, 7, 4,
			3, 0, 4,
			7, 3, 4,
			0, 1, 4,
			1, 5, 4
	};

	indices = 0u;
	glGenBuffers(1, (GLuint*) &(indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesSize, indicesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}