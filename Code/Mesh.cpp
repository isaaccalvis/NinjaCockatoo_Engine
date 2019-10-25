#include <iostream>
#include "Mesh.h"
#include "Application.h"

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

	// Draw Geometry
	if (texture != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, texture->GetBufferPos());
	}
	glEnableClientState(GL_VERTEX_ARRAY);

	glTranslatef(position.x, position.y, position.z);
	glRotated(rotation.Angle() * RADTODEG, rotation.x, rotation.y, rotation.z);
	glScalef(scale.x, scale.y, scale.z);

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (textureCoords != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, textureIndex);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// Draw Normals
	if (App->renderer3D->renderNormals)
	{
		for (int i = 0; i < verticesSize; i++)
		{
			if (normals != nullptr)
				normals[i].Render();
		}
	}

	if (App->renderer3D->renderBoudingBox)
	{
		if (boundingBoxCube != nullptr)
			boundingBoxCube->Render();
	}

	glPopMatrix();
}

void Mesh::SetPosition(math::float3 nPosition)
{
	this->position = nPosition;
}

void Mesh::SetRotation(const math::Quat nRotation)
{
	this->rotation = nRotation;
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