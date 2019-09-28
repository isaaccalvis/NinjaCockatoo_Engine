#include "Application.h"
#include "MeshCustom.h"

#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

MeshCustom::MeshCustom(const char* path) : Mesh()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	if (!scene)
	{
		// TODO: PASAR AQUEST LOG A LOG DE CONSOLA INTERNA
		LOG_IDE("Mesh can not be loaded");
		return;
	}
	allInternalMeshes = new IndividualMesh[scene->mNumMeshes];
	num_Meshes = scene->mNumMeshes;
	for (int a = 0; a < scene->mNumMeshes; a++)
	{
		// Vertices
		allInternalMeshes[a].ind_vertices_array = new GLfloat[scene->mMeshes[a]->mNumVertices * 3];
		int auxCounterVertex = 0;
		for (int i = 0; i < scene->mMeshes[a]->mNumVertices; i++)
		{
			allInternalMeshes[a].ind_vertices_array[auxCounterVertex] = scene->mMeshes[a]->mVertices[i].x;
			auxCounterVertex++;
			allInternalMeshes[a].ind_vertices_array[auxCounterVertex] = scene->mMeshes[a]->mVertices[i].y;
			auxCounterVertex++;
			allInternalMeshes[a].ind_vertices_array[auxCounterVertex] = scene->mMeshes[a]->mVertices[i].z;
			auxCounterVertex++;
		}
		allInternalMeshes[a].verticesSize = scene->mMeshes[a]->mNumVertices;
		glGenBuffers(1, (GLuint*) &(allInternalMeshes[a].individualVertices));
		glBindBuffer(GL_ARRAY_BUFFER, allInternalMeshes[a].individualVertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * scene->mMeshes[a]->mNumVertices * 3, allInternalMeshes[a].ind_vertices_array, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Indices
		allInternalMeshes[a].ind_indices_array = new uint[scene->mMeshes[a]->mNumFaces * 3];
		int auxCounterIndex = 0;
		for (int i = 0; i < scene->mMeshes[a]->mNumFaces; i++)
		{
			allInternalMeshes[a].ind_indices_array[auxCounterIndex] = scene->mMeshes[a]->mFaces[i].mIndices[0];
			auxCounterIndex++;
			allInternalMeshes[a].ind_indices_array[auxCounterIndex] = scene->mMeshes[a]->mFaces[i].mIndices[1];
			auxCounterIndex++;
			allInternalMeshes[a].ind_indices_array[auxCounterIndex] = scene->mMeshes[a]->mFaces[i].mIndices[2];
			auxCounterIndex++;
		}
		allInternalMeshes[a].indicesSize = scene->mMeshes[a]->mNumFaces * 3;
		glGenBuffers(1, (GLuint*) &(allInternalMeshes[a].individualIndices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, allInternalMeshes[a].individualIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * scene->mMeshes[a]->mNumFaces * 3, allInternalMeshes[a].ind_indices_array, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Texture
		if (scene->mMeshes[a]->HasTextureCoords(0))
		{
			allInternalMeshes[a].individual_textureCoor = new GLfloat[scene->mMeshes[a]->mNumVertices * 2];
			for (int i = 0; i < scene->mMeshes[a]->mNumVertices; i++)
			{
				allInternalMeshes[a].individual_textureCoor[i * 2] = scene->mMeshes[a]->mTextureCoords[0][i].x;
				allInternalMeshes[a].individual_textureCoor[(i * 2) + 1] = scene->mMeshes[a]->mTextureCoords[0][i].y;
			}

			glGenBuffers(1, (GLuint*)&allInternalMeshes[a].individualTextureIndex);
			glBindBuffer(GL_ARRAY_BUFFER, allInternalMeshes[a].individualTextureIndex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat*) * 2 * scene->mMeshes[a]->mNumVertices, allInternalMeshes[a].individual_textureCoor, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// Normals TODO: MAKE IT WORK
		if (scene->mMeshes[a]->HasNormals())
		{
			allInternalMeshes[a].individial_normals = new MeshDebugArrow[scene->mMeshes[a]->mNumVertices];
			for (int i = 0; i < scene->mMeshes[a]->mNumVertices; i++)
			{
				allInternalMeshes[a].individial_normals[i].SetDebugArrow(math::float3(scene->mMeshes[a]->mVertices[i].x, scene->mMeshes[a]->mVertices[i].y, scene->mMeshes[a]->mVertices[i].z), math::float3(scene->mMeshes[a]->mVertices[i].x + scene->mMeshes[a]->mNormals[i].x , scene->mMeshes[a]->mVertices[i].y + scene->mMeshes[a]->mNormals[i].y, scene->mMeshes[a]->mVertices[i].z + scene->mMeshes[a]->mNormals[i].z));
			}
		}

		// Bounding Box
		allInternalMeshes[a].individual_boundingBox.SetNegativeInfinity();
		allInternalMeshes[a].individual_boundingBox.Enclose((const math::float3*)allInternalMeshes[a].ind_vertices_array, allInternalMeshes[a].verticesSize);
		allInternalMeshes[a].individual_cubeBouncingBox = new MeshDebugCube(allInternalMeshes[a].individual_boundingBox.CenterPoint(), allInternalMeshes[a].individual_boundingBox.Size());
	}
	// Set texture
	if (App->textures->last_texture != nullptr)
		texture = App->textures->last_texture;
}

MeshCustom::~MeshCustom()
{

}

void MeshCustom::Render()
{
	for (int i = 0; i < num_Meshes; i++) {

		glPushMatrix();

		// Draw Geometry
		if (texture != nullptr)
		{
			glBindTexture(GL_TEXTURE_2D, texture->BufferPos);
		}
		glEnableClientState(GL_VERTEX_ARRAY);

		glTranslatef(position.x, position.y, position.z);
		glScalef(scale.x, scale.y, scale.z);

		glBindBuffer(GL_ARRAY_BUFFER, allInternalMeshes[i].individualVertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		if (allInternalMeshes[i].individual_textureCoor != nullptr)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, allInternalMeshes[i].individualTextureIndex);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, allInternalMeshes[i].individualIndices);
		glDrawElements(GL_TRIANGLES, allInternalMeshes[i].indicesSize, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		// Draw Normals
		for (int i = 0; i < num_Meshes; i++)
			for (int e = 0; e < allInternalMeshes[i].verticesSize; e++)
			{
				if (allInternalMeshes[i].individial_normals != nullptr)
					allInternalMeshes[i].individial_normals[e].Render();
			}
		allInternalMeshes[i].individual_cubeBouncingBox->Render();

		glPopMatrix();
	}
}

MeshCustom::IndividualMesh::~IndividualMesh()
{
	glDeleteBuffers(1, (GLuint*)&individualVertices);
	glDeleteBuffers(1, (GLuint*)&individualIndices);
	glDeleteBuffers(1, (GLuint*)&individual_textureCoor);
	if (ind_vertices_array != nullptr)
		delete[] ind_vertices_array;
	if (ind_indices_array != nullptr)
		delete[] ind_indices_array;
}