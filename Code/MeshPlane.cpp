#include "MeshPlane.h"

#include "mmgr/mmgr.h"

MeshPlane::MeshPlane()
{
	verticesSize = 12;
	verticesArray = new GLfloat[verticesSize]{
		-0.5f,  -0.5f, -0.5f,
		0.5f,  -0.5f, -0.5f,
		-0.5f,  -0.5f, 0.5f,
		0.5f,  -0.5f, 0.5f
	};

	vertices = 0u;
	glGenBuffers(1, (GLuint*) &(vertices));
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesSize, verticesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	indicesSize = 6;
	indicesArray = new unsigned int[indicesSize] {
		0,2,1,
		2,3,1
	};

	indices = 0u;
	glGenBuffers(1, (GLuint*) &(indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesSize, indicesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}