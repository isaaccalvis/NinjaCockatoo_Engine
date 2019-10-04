#ifndef __DebugArrow_H__
#define __DebugArrow_H__

#include "MathGeoLib-1.5\src\Math\float3.h"
#include "glew-2.1.0/include/GL/glew.h"

class DebugArrow
{
public:
	DebugArrow();
	DebugArrow(math::float3 origin, math::float3 end);
	~DebugArrow();

	void Render();

	void SetDebugArrow(math::float3 origin, math::float3 end);

public:
	// Vertices
	unsigned int vertices = 0u;
	unsigned int verticesSize = 0;
	GLfloat* verticesArray = nullptr;

	// Indices
	unsigned int indices = 0u;
	unsigned int indicesSize = 0;
	unsigned int* indicesArray = nullptr;
};

#endif