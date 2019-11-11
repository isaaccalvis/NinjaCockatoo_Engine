#ifndef __Mesh_H__
#define __Mesh_H__

#include "Globals.h"
#include "ModuleTextures.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "MathGeoLib-1.5/src/MathGeoLib.h"
#include "DebugArrow.h"
#include "DebugCube.h"

#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

class Mesh
{
public:
	Mesh();
	Mesh(MESH_TYPE type);
	Mesh(const aiScene* scene, const aiNode* node, const int num = 0);
	virtual ~Mesh();

	virtual void Render(Texture* texture = nullptr, math::float4x4 globalMatrix = math::float4x4::identity);

	// Buffers
	void GenerateIndicesBuffer();
	void GenerateVerticesBuffer();
	void GenerateTextureCoorBuffer();
	void GenerateNormalsBuffers();
	void ClearIndicesBuffer();
	void ClearVerticesBuffer();
	void ClearTextureCoorBuffer();

	// Indices, Vertices, TextureCoords, Normals
	void ClearIndicesArray();
	void ClearVerticesArray();
	void ClearTextureCoorArray();
	void ClearNormalsArray();

	// Indices, Vertices, TextureCoords, Normals Size
	void SetIndicesSize(unsigned int size);
	void SetVerticesSize(unsigned int size);
	void SetTextureCoorSize(unsigned int size);
	void SetNormalsSize(unsigned int size);
	unsigned int GetIndicesSize() const;
	unsigned int GetVerticesSize() const;
	unsigned int GetTextureCoorSize() const;
	unsigned int GetNormalsSize() const;

public:
	MESH_TYPE type;

	GLfloat* verticesArray = nullptr;
	unsigned int* indicesArray = nullptr;
	GLfloat* textureCoords = nullptr;
	DebugArrow* normals = nullptr;

	std::vector<float3> vectorVertex;

protected:
	// Vertices
	unsigned int vertices = 0u;
	unsigned int verticesSize = 0;

	// Indices
	unsigned int indices = 0u;
	unsigned int indicesSize = 0;
	
	// Texture
	unsigned int textureIndex = 0u;
	unsigned int textureCoorSize = 0u;

	// Normals
	unsigned int normalsSize = 0u;
};

#endif