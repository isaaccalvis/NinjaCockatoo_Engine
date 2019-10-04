#ifndef __Primitive_H__
#define __Primitive_H__

#include "Globals.h"
#include "Mesh.h"

class MeshPrimitive : public Mesh
{
public:
	MeshPrimitive();
	MeshPrimitive(PRIMITIVE_MESHES type);

	PRIMITIVE_MESHES GetType() const;
	void SetType(PRIMITIVE_MESHES type);
private:
	PRIMITIVE_MESHES type;
};

#endif