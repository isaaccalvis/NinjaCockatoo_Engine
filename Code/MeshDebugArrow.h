#ifndef __MeshDebugArrow_H__
#define __MeshDebugArrow_H__

#include "Mesh.h"

class MeshDebugArrow : public Mesh
{
public:
	MeshDebugArrow();
	MeshDebugArrow(math::float3 origin, math::float3 end);
	~MeshDebugArrow();

	void Render();

	void SetDebugArrow(math::float3 origin, math::float3 end);
};

#endif