#ifndef __Component_H__
#define __Component_H__

#include "GameObject.h"

class C_Transform;
class C_Mesh;
class C_Material;

class Component
{
public:
	Component(GameObject* parent, COMPONENT_TYPE type);
	~Component();

	virtual void Update(float dt) = 0;

	void SetEnable(bool set);
	bool GetEnabled();

	C_Transform* GetComponentTransform() const;
	C_Mesh* GetComponentMesh() const;
	C_Material* GetComponentMaterial() const;

public:
	COMPONENT_TYPE type;
	bool active = true;
	GameObject* parent;
};

#endif