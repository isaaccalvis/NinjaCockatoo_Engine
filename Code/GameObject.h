#ifndef __GameObject_H__
#define __GameObject_H__

#include <vector>
#include <string>
#include "Globals.h"

enum COMPONENT_TYPE
{
	COMPONENT_NONE = 0,
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_MATERIAL
};

class Component;

class GameObject
{
public:
	GameObject();
	GameObject(const char* name, GameObject* parent);
	~GameObject();

	void Update(float dt);

	Component* CreateComponent(COMPONENT_TYPE type, const char* name = nullptr);
	Component* GetComponent(COMPONENT_TYPE type, const char* name = nullptr);

	GameObject* GetParent() const;
	void SetParent(GameObject* parent);

	int CountChild();

public:
	const char* name = nullptr;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
	bool active = true;

private:
	GameObject* parent = nullptr;
};

#endif