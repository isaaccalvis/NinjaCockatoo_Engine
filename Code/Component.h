#ifndef __Component_H__
#define __Component_H__

#include "GameObject.h"

class C_Transform;
class C_Mesh;
class C_Material;
class C_Camera;

class Component
{
public:
	Component(GameObject* parent, COMPONENT_TYPE type);
	virtual ~Component();

	virtual void Update(float dt) = 0;

	void SetEnable(bool set);
	bool GetEnabled() const;

	C_Transform* GetComponentAsTransform() const;
	C_Mesh* GetComponentAsMesh() const;
	C_Material* GetComponentAsMaterial() const;
	C_Camera* GetComponentAsCamera() const;

	virtual void OnSaveJson(JSON_Object* object) {};
	virtual void OnLoadJson(JSON_Object* object) {};

public:
	COMPONENT_TYPE type = COMPONENT_NONE;
	bool active = true;
	bool isUnique = false;
	GameObject* parent = nullptr;
	const char* name;
};

#endif