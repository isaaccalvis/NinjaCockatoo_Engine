#ifndef __MeshCube_H__
#define __MeshCube_H__

#include "Mesh.h"

class MeshCube : public Mesh
{
public:
	MeshCube(int size);
	~MeshCube();


public:
	int size = 1;

};

#endif