#ifndef __C_Mesh_H__
#define __C_Mesh_H__

#include "Component.h"
#include "Mesh.h"

class C_Mesh : public Component
{
public:
	C_Mesh(GameObject* parent);
	~C_Mesh();

	void Update(float dt);
	
public:
	Mesh* mesh;
};

#endif