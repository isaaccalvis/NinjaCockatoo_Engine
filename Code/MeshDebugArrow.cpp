#include "MeshDebugArrow.h"

MeshDebugArrow::MeshDebugArrow()
{

}

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
	indicesArray = new unsigned int[indicesSize] {
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

void MeshDebugArrow::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glTranslatef(position.x, position.y, position.z);
	glScalef(scale.x, scale.y, scale.z);

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void MeshDebugArrow::SetDebugArrow(math::float3 origin, math::float3 end)
{
	// Clean old Arrow
	delete[] verticesArray;
	delete[] indicesArray;
	glDeleteBuffers(1, (GLuint*)&vertices);
	glDeleteBuffers(1, (GLuint*)&indices);

	// Set new Arrow
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
	indicesArray = new unsigned int[indicesSize] {
		0, 1
	};

	indices = 0u;
	glGenBuffers(1, (GLuint*) &(indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesSize, indicesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}