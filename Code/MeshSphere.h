#ifndef __MeshSphere_H__
#define __MeshSphere_H__

#include "Mesh.h"

class MeshSphere : public Mesh
{
public:
	MeshSphere(unsigned int rings, unsigned int sectors);
};

#endif