#include "MeshSphere.h"

// RADIUS = 1.0f
//TODO: save info at vertiesArray & indicesArray
MeshSphere::MeshSphere(unsigned int rings, unsigned int sectors)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);

	verticesSize = rings * sectors * 3;
	GLfloat* vertices_array = new GLfloat[verticesSize];

	for (int r = 0; r < rings; r++)
	{
		for (int s = 0; s < sectors; s++)
		{
			*vertices_array++ = cos(2 * pi * s * S) * sin(pi * r * R) * 1.;	// x 
			*vertices_array++ = sin(-pi / 2 + pi * r * R) * 1.;				// y
			*vertices_array++ = sin(2 * pi * s * S) * sin(pi * r * R) * 1.;	// z
		}
	}
	vertices = 0u;
	glGenBuffers(1, (GLuint*) &(vertices));
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesSize, vertices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	indicesSize = rings * sectors * 3 * 2;
	unsigned int* indices_array = new unsigned int[indicesSize];
	for (int r = 0; r < rings; ++r)
	{
		for (int s = 0; s < sectors; ++s) {
			//*indices_array++ = (r + 1) * sectors + s;
			//*indices_array++ = (r + 1) * sectors + (s + 1);
			//*indices_array++ = r * sectors + (s + 1);
			//*indices_array++ = r * sectors + s;
			*indices_array++ = (r * sectors) + s;
			*indices_array++ = ((r + 1) * sectors) + s;
			*indices_array++ = ((r + 1) * sectors) + (s + 1);

			*indices_array++ = (r * sectors) + s;
			*indices_array++ = ((r + 1) * sectors) + (s + 1);
			*indices_array++ = (r * sectors) + (s + 1);
		}
	}
	indices = 0u;
	glGenBuffers(1, (GLuint*) &(indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesSize, indices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}