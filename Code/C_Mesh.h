#ifndef __C_Mesh_H__
#define __C_Mesh_H__

#include "Component.h"

class C_Mesh : Component
{
public:
	C_Mesh(GameObject* parent);
	~C_Mesh();

	void Update(float dt);

};

#endif