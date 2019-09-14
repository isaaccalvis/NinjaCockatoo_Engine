#include "Mesh.h"

Mesh::Mesh()
{}

Mesh::~Mesh()
{}

void Mesh::Render()
{
	switch (type_shape)
	{
	case DRAW_TYPE_SHAPE::TRIANGLE:
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
		break;
	case DRAW_TYPE_SHAPE::QUAD:
		glEnableClientState(GL_VERTEX_ARRAY);
		glTranslatef(position.x, position.y, position.z);
		glScaled(scale.x, scale.y, scale.z);
		glBindBuffer(GL_ARRAY_BUFFER, vertices);
		glVertexPointer(3, GL_FLOAT, 0, nullptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
		glDrawElements(GL_QUADS, indicesSize, GL_UNSIGNED_INT, nullptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		break;
	}
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