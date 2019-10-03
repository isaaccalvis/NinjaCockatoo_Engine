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
	GameObject(std::string name, GameObject* parent = nullptr);
	~GameObject();

	void Update(float dt);

	void SetName(const char* name);
	void SetParent(GameObject* parent);

	std::string GetName() const;
	GameObject* GetParent() const;

	Component* CreateComponent(COMPONENT_TYPE type, std::string name = nullptr);
	Component* GetComponent(COMPONENT_TYPE type, std::string name = nullptr);

private:
	std::string name;
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
};

#endif