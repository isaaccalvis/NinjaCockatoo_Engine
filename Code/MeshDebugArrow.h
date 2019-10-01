#ifndef __MeshDebugArrow_H__
#define __MeshDebugArrow_H__

#include "Mesh.h"

// TODO: LES NORMALS LES PINTEM D'UNA ALTRE MANERA ?
class MeshDebugArrow : public Mesh
{
public:
	MeshDebugArrow();
	MeshDebugArrow(math::float3 origin, math::float3 end);

	void Render();

	void SetDebugArrow(math::float3 origin, math::float3 end);
};

#endif