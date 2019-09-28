#ifndef __MeshDebugCube_H__
#define __MeshDebugCube_H__

#include "Mesh.h"

class MeshDebugCube : public Mesh
{
public:
	MeshDebugCube(math::float3 center = math::float3(0,0,0), math::float3 resize = math::float3(1, 1, 1));

	void Render();
};

#endif