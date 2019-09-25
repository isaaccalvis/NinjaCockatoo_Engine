#ifndef __MeshCustom_H__
#define __MeshCustom_H__

#include "Mesh.h"
#include "MeshDebugArrow.h"

class MeshCustom : public Mesh
{
public:
	MeshCustom(const char* path);
	~MeshCustom();

	void Render();

private:
	class IndividualMesh {
	public:
		IndividualMesh() {}
		~IndividualMesh();

		uint individualVertices = 0u;
		uint individualIndices = 0u;
		uint individualTextureCoors = 0u;

		GLfloat* ind_vertices_array = nullptr;
		uint* ind_indices_array = nullptr;

		uint verticesSize = 0u;
		uint indicesSize = 0u;
		MeshDebugArrow* normals = nullptr;
	};

	IndividualMesh* allInternalMeshes = nullptr;
	uint num_Meshes = 0u;
};

#endif