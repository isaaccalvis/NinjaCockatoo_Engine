#include <iostream>
#include "Mesh.h"
#include "Application.h"
#include "par_shapes.h"

Mesh::Mesh()
{}

Mesh::Mesh(MESH_TYPE type)
{
	verticesSize = 0;
	indicesSize = 0;
	par_shapes_mesh* primitive;
	this->type = type;

	switch (type)
	{
	case MESH_TYPE::PRIMITIVE_CUBE:
		primitive = par_shapes_create_cube();
		break;
	case MESH_TYPE::PRIMITIVE_PLANE:
		primitive = par_shapes_create_plane(1, 1);
		break;
	case MESH_TYPE::PRIMITIVE_SPHERE:
		primitive = par_shapes_create_parametric_sphere(10, 10);
		break;
	}
	vectorVertex.resize(primitive->npoints);
	int a = 0;
	for (int i = 0; i < primitive->npoints * 3; i++)
	{
		vectorVertex[a].Set(primitive->points[i], primitive->points[i++], primitive->points[i++]);
		a++;
	}

	verticesSize = primitive->npoints;
	verticesArray = new GLfloat[verticesSize * 3];
	for (int i = 0; i < verticesSize * 3; i++)
	{
		verticesArray[i] = primitive->points[i];
	}

	indicesSize = primitive->ntriangles;
	indicesArray = new unsigned int[indicesSize * 3];
	for (int i = 0; i < indicesSize * 3; i++)
	{
		indicesArray[i] = primitive->triangles[i];
	}

	textureCoords = new GLfloat[primitive->ntriangles * 2];
	if (primitive->tcoords != nullptr)
	{
		for (int i = 0; i < primitive->ntriangles * 2; i++)
		{
			textureCoords[i] = primitive->tcoords[i];
		}
	}
	else
	{
		for (int i = 0; i < primitive->ntriangles * 2; i++)
		{
			textureCoords[i] = 0;
			textureCoords[i++] = 0;
			textureCoords[i++] = 1;
			textureCoords[i++] = 0;
			textureCoords[i++] = 0;
			textureCoords[i++] = 1;

			textureCoords[i++] = 0;
			textureCoords[i++] = 1;
			textureCoords[i++] = 1;
			textureCoords[i++] = 0;
			textureCoords[i++] = 1;
			textureCoords[i++] = 1;
		}
	}

	GenerateVerticesBuffer();
	GenerateIndicesBuffer();
	GenerateTextureCoorBuffer();
}

Mesh::Mesh(const aiScene* scene, const aiNode* node, const int num)
{
	this->type = MESH_TYPE::CUSTOM_MESH;

	// Vertices
	verticesSize = scene->mMeshes[node->mMeshes[num]]->mNumVertices;
	verticesArray = new GLfloat[verticesSize * 3];
	int auxCounterVertex = 0;
	for (int i = 0; i < verticesSize; i++)
	{
		verticesArray[auxCounterVertex] = scene->mMeshes[node->mMeshes[num]]->mVertices[i].x;
		auxCounterVertex++;
		verticesArray[auxCounterVertex] = scene->mMeshes[node->mMeshes[num]]->mVertices[i].y;
		auxCounterVertex++;
		verticesArray[auxCounterVertex] = scene->mMeshes[node->mMeshes[num]]->mVertices[i].z;
		auxCounterVertex++;
	}

	vectorVertex.resize(verticesSize);
	int a = 0;
	for (int i = 0; i < verticesSize * 3; i++)
	{
		vectorVertex[a].Set(verticesArray[i], verticesArray[i++], verticesArray[i++]);
		a++;
	}

	// Indices
	indicesSize = scene->mMeshes[(*node->mMeshes)]->mNumFaces;
	indicesArray = new uint[indicesSize * 3];
	int auxCounterIndex = 0;
	for (int i = 0; i < indicesSize; i++)
	{
		indicesArray[auxCounterIndex] = scene->mMeshes[node->mMeshes[num]]->mFaces[i].mIndices[0];
		auxCounterIndex++;
		indicesArray[auxCounterIndex] = scene->mMeshes[node->mMeshes[num]]->mFaces[i].mIndices[1];
		auxCounterIndex++;
		indicesArray[auxCounterIndex] = scene->mMeshes[node->mMeshes[num]]->mFaces[i].mIndices[2];
		auxCounterIndex++;
	}

	GenerateVerticesBuffer();
	GenerateIndicesBuffer();

	// Texture
	textureCoorSize = 0;
	if (scene->mMeshes[node->mMeshes[num]]->HasTextureCoords(0))
	{
		textureCoorSize = scene->mMeshes[node->mMeshes[num]]->mNumFaces * 2 * 3;
		textureCoords = new GLfloat[textureCoorSize];
		for (int i = 0; i < scene->mMeshes[node->mMeshes[num]]->mNumFaces * 3; i++)
		{
			textureCoords[i * 2] = scene->mMeshes[node->mMeshes[num]]->mTextureCoords[0][i].x;
			textureCoords[(i * 2) + 1] = scene->mMeshes[node->mMeshes[num]]->mTextureCoords[0][i].y;
		}

		GenerateTextureCoorBuffer();
	}

	// Normals TODO: MAKE IT WORK
	normalsSize = scene->mMeshes[node->mMeshes[num]]->mNumVertices;
	if (scene->mMeshes[node->mMeshes[num]]->HasNormals())
	{
		normals = new DebugArrow[scene->mMeshes[node->mMeshes[num]]->mNumVertices];
		for (int i = 0; i < scene->mMeshes[node->mMeshes[num]]->mNumVertices; i++)
		{
			normals[i].SetDebugArrow(math::float3(scene->mMeshes[node->mMeshes[num]]->mVertices[i].x, scene->mMeshes[node->mMeshes[num]]->mVertices[i].y, scene->mMeshes[node->mMeshes[num]]->mVertices[i].z), math::float3(scene->mMeshes[node->mMeshes[num]]->mVertices[i].x + scene->mMeshes[node->mMeshes[num]]->mNormals[i].x, scene->mMeshes[node->mMeshes[num]]->mVertices[i].y + scene->mMeshes[node->mMeshes[num]]->mNormals[i].y, scene->mMeshes[node->mMeshes[num]]->mVertices[i].z + scene->mMeshes[node->mMeshes[num]]->mNormals[i].z));
		}
	}
}

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
}

void Mesh::Render(Texture* texture, math::float4x4 globalMatrix)
{
	glPushMatrix();

	// Draw Geometry
	if (texture != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, texture->GetBufferPos());
	}

	glEnableClientState(GL_VERTEX_ARRAY);

	glMultMatrixf(globalMatrix.Transposed().ptr());

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (textureCoords != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, textureIndex);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glDrawElements(GL_TRIANGLES, indicesSize * 3, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// Draw Normals
	if (App->renderer3D->renderNormals)
	{
		for (int i = 0; i < normalsSize; i++)
		{
			if (normals != nullptr)
				normals[i].Render();
		}
	}
	glPopMatrix();
}

void Mesh::ClearIndicesArray()
{
	if (indicesArray != nullptr)
		delete[] indicesArray;
}

void Mesh::ClearVerticesArray()
{
	if (verticesArray != nullptr)
		delete[] verticesArray;
}

void Mesh::ClearTextureCoorArray()
{
	if (textureCoords != nullptr)
		delete[] textureCoords;
}

void Mesh::ClearNormalsArray()
{
	if (normals != nullptr)
		delete[] normals;
}

void Mesh::GenerateIndicesBuffer()
{
	ClearIndicesBuffer();
	indices = 0u;
	glGenBuffers(1, (GLuint*) &(indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indicesSize * 3, indicesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::GenerateVerticesBuffer()
{
	ClearVerticesBuffer();
	vertices = 0u;
	glGenBuffers(1, (GLuint*) &(vertices));
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesSize * 3, verticesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	vectorVertex.resize(verticesSize);
	int a = 0;
	for (int i = 0; i < verticesSize * 3; i++)
	{
		vectorVertex[a].Set(verticesArray[i], verticesArray[i++], verticesArray[i++]);
		a++;
	}
}

void Mesh::GenerateTextureCoorBuffer()
{
	ClearTextureCoorBuffer();
	glGenBuffers(1, (GLuint*)&textureIndex);
	glBindBuffer(GL_ARRAY_BUFFER, textureIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat*) * textureCoorSize, textureCoords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::GenerateNormalsBuffers()
{
	for (int i = 0; i < GetNormalsSize(); i++)
	{
		normals[i].ClearBuffers();
		normals[i].GenerateBuffers();
	}
}

void Mesh::ClearIndicesBuffer()
{
	glDeleteBuffers(1, (GLuint*)&indices);
}

void Mesh::ClearVerticesBuffer()
{
	glDeleteBuffers(1, (GLuint*)&vertices);
}

void Mesh::ClearTextureCoorBuffer()
{
	glDeleteBuffers(1, (GLuint*)&textureIndex);
}

void Mesh::SetIndicesSize(unsigned int size)
{
	this->indicesSize = size;
}

void Mesh::SetVerticesSize(unsigned int size)
{
	this->verticesSize = size;
}

void Mesh::SetTextureCoorSize(unsigned int size)
{
	this->textureCoorSize = size;
}

void Mesh::SetNormalsSize(unsigned int size)
{
	this->normalsSize = size;
}

unsigned int Mesh::GetIndicesSize() const
{
	return indicesSize;
}

unsigned int Mesh::GetVerticesSize() const
{
	return verticesSize;
}

unsigned int Mesh::GetTextureCoorSize() const
{
	return textureCoorSize;
}

unsigned int Mesh::GetNormalsSize() const
{
	return normalsSize;
}

float* Mesh::GetNormalsVertices(int a) const
{
	float* ret = new float[6];
	for (int i = 0; i < 6; i++)
	{
		ret[i] = normals[a].verticesArray[i];
	}
	return ret;
}