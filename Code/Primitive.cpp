#include "Primitive.h"
#include "par_shapes.h"

MeshPrimitive::MeshPrimitive() : Mesh()
{}

MeshPrimitive::MeshPrimitive(PRIMITIVE_MESHES type) : Mesh()
{

	verticesSize = 0;
	indicesSize = 0;
	par_shapes_mesh* primitive;
	switch (type)
	{
	case PRIMITIVE_MESHES::PRIMITIVE_CUBE:
		primitive = par_shapes_create_cube();
		break;
	case PRIMITIVE_MESHES::PRIMITIVE_PLANE:
		primitive = par_shapes_create_plane(1, 1);
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
		primitive = par_shapes_create_cylinder(16, 8);
		break;
	case PRIMITIVE_MESHES::PRIMITIVE_SPHERE:
		primitive = par_shapes_create_subdivided_sphere(3);
		break;
	}

	verticesSize = primitive->npoints * 3;
	verticesArray = new GLfloat[verticesSize];
	for (int i = 0; i < verticesSize; i++)
	{
		verticesArray[i] = primitive->points[i];
	}

	indicesSize = primitive->ntriangles * 3;
	indicesArray = new unsigned int[indicesSize];
	for (int i = 0; i < indicesSize; i++)
	{
		indicesArray[i] = primitive->triangles[i];
	}

	textureCoords = new GLfloat[primitive->npoints * 2];
	textureCoords = primitive->tcoords;

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

	// Bounding Box
	//boundingBox.SetNegativeInfinity();
	//boundingBox.Enclose((const math::float3*)verticesArray, verticesSize);
	//boundingBoxCube = new DebugCube(boundingBox.CenterPoint(), boundingBox.Size());
}

PRIMITIVE_MESHES MeshPrimitive::GetType() const
{
	return type;
}

void MeshPrimitive::SetType(PRIMITIVE_MESHES type)
{
	this->type = type;
}