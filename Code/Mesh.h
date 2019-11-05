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

	virtual void Render(Texture* texture = nullptr);

	// Transform
	void SetPosition(const math::float3 nPosition);
	void SetRotation(const math::Quat nRotation);
	void SetScale(const math::float3 nScale);

	math::float3 GetPosition() const;
	math::Quat GetRotation() const;
	math::float3 GetScale() const;

	// Indices, Vertices, TextureCoords, Normals
	void SetIndicesArray(unsigned int* indices);
	void SetVerticesArray(GLfloat* vertices);
	void SetTextureCoorArray(GLfloat* textureCoor);
	void SetNormalsArray(DebugArrow* normals);

	unsigned int* GetIndicesArray() const;
	GLfloat* GetVerticesArray() const;
	GLfloat* GetTextureCoorArray() const;
	DebugArrow* GetNormalsArray() const;

	void ClearIndicesArray();
	void ClearVerticesArray();
	void ClearTextureCoorArray();
	void ClearNormalsArray();

	void GenerateIndicesBuffer();
	void GenerateVerticesBuffer();
	void GenerateTextureCoorBuffer();

	void ClearIndicesBuffer();
	void ClearVerticesBuffer();
	void ClearTextureCoorBuffer();

	void SetIndicesSize(unsigned int size);
	void SetVerticesSize(unsigned int size);
	void SetTextureCoorSize(unsigned int size);
	void SetNormalsSize(unsigned int size);

	unsigned int GetIndicesSize() const;
	unsigned int GetVerticesSize() const;
	unsigned int GetTextureCoorSize() const;
	unsigned int GetNormalsSize() const;

	AABB GetBoundingBox();

	MESH_TYPE type;

protected:
	// Transform
	math::float3 position = math::float3::zero;
	math::Quat rotation = math::Quat::identity;
	math::float3 scale = math::float3::one;

	// Vertices
	unsigned int vertices = 0u;
	unsigned int verticesSize = 0;
	GLfloat* verticesArray = nullptr;
	std::vector<float3> vectorVertex;

	// Indices
	unsigned int indices = 0u;
	unsigned int indicesSize = 0;
	unsigned int* indicesArray = nullptr;
	
	// Texture
	unsigned int textureIndex = 0u;
	unsigned int textureCoorSize = 0u;
	GLfloat* textureCoords = nullptr;

	// Normals
	unsigned int normalsSize = 0u;
	DebugArrow* normals = nullptr;

	// Bounding Box & AABB
	AABB boundingBox;
	DebugCube* boundingBoxCube = nullptr;
};

#endif