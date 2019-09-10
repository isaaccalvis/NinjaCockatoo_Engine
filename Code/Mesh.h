#ifndef __Mesh_H__
#define __Mesh_H__

#include "glew-2.1.0/include/GL/glew.h"

class Mesh
{
public:
	Mesh();
	~Mesh(); // TODO: DESTRUCTOR

	void Render();

public:
	int verticesSize = 0;
	unsigned int vertices = 0u;
	int indicesSize = 0;
	unsigned int indices = 0u;

};

#endif