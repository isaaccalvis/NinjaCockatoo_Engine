#ifndef __Grid_H__
#define __Grid_H__

#include "MathGeoLib-1.5\src\Math\float3.h"
#include "glew-2.1.0/include/GL/glew.h"

class Grid
{
public:
	Grid(int nScale = 1);
	~Grid();

	void Render();

public:
	// Vertices
	unsigned int vertices = 0u;
	unsigned int verticesSize = 0;
	GLfloat* verticesArray = nullptr;

	// Indices
	unsigned int indices = 0u;
	unsigned int indicesSize = 0;
	unsigned int* indicesArray = nullptr;

	math::float3 position = { 0,0,0 };
	//math::float3 rotation = { 0,0,0 }; 	// TODO: MAKE ROTATION
	math::float3 scale = { 1,1,1 };
};

#endif