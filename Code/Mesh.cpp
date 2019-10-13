#include <iostream>
#include "Mesh.h"

Mesh::Mesh()
{}

Mesh::~Mesh()
{
	glDeleteBuffers(1, (GLuint*)&vertices);
	glDeleteBuffers(1, (GLuint*)&indices);
	glDeleteBuffers(1, (GLuint*)&textureIndex);
	if (verticesArray != nullptr)
		delete[] verticesArray;
	if (indicesArray != nullptr)
		delete[] indicesArray;
	if (textureCoords != nullptr)
		delete[] textureCoords;
	if (normals != nullptr)
		delete[] normals;
	if (boundingBoxCube != nullptr)
		delete boundingBoxCube;
}

void Mesh::Render(Texture* texture)
{
	glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);

	glTranslatef(position.x, position.y, position.z);
	glScaled(scale.x, scale.y, scale.z);

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

void Mesh::SetPosition(math::float3 nPosition)
{
	this->position = nPosition;
}

void Mesh::SetScale(math::float3 nScale)
{
	this->scale = nScale;
}

math::float3 Mesh::GetPosition() const
{
	return position;
}

math::float3 Mesh::GetScale() const
{
	return scale;
}

unsigned int Mesh::GetIndicesSize() const
{
	return indicesSize;
}

unsigned int Mesh::GetVerticesSize() const
{
	return verticesSize;
}

AABB Mesh::GetBoundingBox()
{
	return boundingBox;
}