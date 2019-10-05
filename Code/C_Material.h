#ifndef __C_Material_H__
#define __C_Material_H__

#include "Component.h"
#include "ModuleTextures.h"

class C_Material : public Component
{
public:
	C_Material(GameObject* parent);
	~C_Material();

	void Update(float dt);

public:
	Texture* texture = nullptr;
};

#endif