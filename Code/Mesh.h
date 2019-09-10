#ifndef __MESH_H__
#define __MESH_H__

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Render();

public:
	int verticesSize = 0;
	unsigned int vertices = 0u;
	int indicesSize = 0;
	unsigned int indices = 0u;

};

#endif