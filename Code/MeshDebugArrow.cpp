#include "MeshDebugArrow.h"

MeshDebugArrow::MeshDebugArrow(math::float3 origin, math::float3 end) : Mesh()
{
	verticesSize = 6;
	verticesArray = new GLfloat[verticesSize]{
		origin.x, origin.y, origin.z,
		end.x, end.y, end.z
	};

	vertices = 0u;
	glGenBuffers(1, (GLuint*) &(vertices));
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesSize, verticesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	indicesSize = 2;
	unsigned int* indicesArray = new unsigned int[indicesSize] {
		0,1
	};

	indices = 0u;
	glGenBuffers(1, (GLuint*) &(indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesSize, indicesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

MeshDebugArrow::~MeshDebugArrow()
{

}