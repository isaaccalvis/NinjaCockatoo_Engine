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

	const char* GetName() const;
	void SetName(const char* name);

	bool IsActive() const;
	void SetActive(bool set);


	// Parent
	GameObject*		GetParent() const;
	void			SetParent(GameObject* parent);

	// Children
	void			AddChildren(GameObject* child);
	void			DeleteChild(GameObject* child);
	void			QuitChildFromVector(GameObject* child);
	GameObject*		GetChild(int num) const;
	int				CountChild();

	// Components
	Component*		CreateComponent(COMPONENT_TYPE type, const char* name = nullptr);
	Component*		GetComponent(COMPONENT_TYPE type, const char* name = nullptr);
	Component*		GetComponent(int num) const;
	void			DeleteComponent(COMPONENT_TYPE type, const char* name = nullptr);
	int				CountComponents() const;

	void CalculateOwnBoundingBox();
	void CalculateChilldBoundingBox();

private:
	GameObject* parent = nullptr;
	std::string name = "";
	bool active = true;

	std::vector<GameObject*> children;
	std::vector<Component*> components;

public:
	math::AABB ownBoundingBox;
	math::AABB childBoundingBox;
};

#endif