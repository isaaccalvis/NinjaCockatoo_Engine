#include "Primitive.h"

MeshPrimitive::MeshPrimitive() : Mesh()
{}

MeshPrimitive::MeshPrimitive(PRIMITIVE_MESHES type) : Mesh()
{
	verticesSize = 0;
	indicesSize = 0;
	switch (type)
	{
	case PRIMITIVE_MESHES::PRIMITIVE_CUBE:
	{
		verticesSize = 24;
		verticesArray = new GLfloat[verticesSize]{
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f
		};
		indicesSize = 36;
		indicesArray = new unsigned int[indicesSize] {
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
	}
	break;
	case PRIMITIVE_MESHES::PRIMITIVE_PLANE:
	{
		verticesSize = 12;
		verticesArray = new GLfloat[verticesSize]{
			-0.5f,  -0.5f, -0.5f,
			 0.5f,  -0.5f, -0.5f,
			-0.5f,  -0.5f,  0.5f,
			 0.5f,  -0.5f,  0.5f
		};

		indicesSize = 6;
		indicesArray = new unsigned int[indicesSize] {
			0, 2, 1,
				2, 3, 1
		};
	}
	break;
	case PRIMITIVE_MESHES::PRIMITIVE_FRUSTRUM:
	{
		verticesSize = 24;
		verticesArray = new GLfloat[verticesSize]{
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.25f, 0.5f, -0.25f,
			 0.25f, 0.5f, -0.25f,
			 0.25f, 0.5f,  0.25f,
			-0.25f, 0.5f,  0.25f
		};

		indicesSize = 36;
		indicesArray = new unsigned int[indicesSize] {
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
	}
	break;
	case PRIMITIVE_MESHES::PRIMITIVE_CYLINDER:
	{
		verticesSize = 24;
		verticesArray = new GLfloat[verticesSize];

		float x;
		float y;
		float angle;

		for (angle = 0.0; angle < 2 * pi; angle++) {
			*verticesArray++ = cos(angle);
			*verticesArray++ = sin(angle);
			*verticesArray++ = 1;
		}

		indicesSize = 36;
		indicesArray = new unsigned int[indicesSize] {
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
	}
	break;
	case PRIMITIVE_MESHES::PRIMITIVE_SPHERE:
	{

	}
	break;
	default: {}
		break;
	}

	vertices = 0u;
	glGenBuffers(1, (GLuint*) &(vertices));
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesSize, verticesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	indices = 0u;
	glGenBuffers(1, (GLuint*) &(indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesSize, indicesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


PRIMITIVE_MESHES MeshPrimitive::GetType() const
{
	return type;
}

void MeshPrimitive::SetType(PRIMITIVE_MESHES type)
{
	this->type = type;
}