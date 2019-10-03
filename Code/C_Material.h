#ifndef __C_Material_H__
#define __C_Material_H__

#include "Component.h"

class C_Material : Component
{
public:
	C_Material(GameObject* parent);
	~C_Material();

	void Update(float dt);

};

#endif