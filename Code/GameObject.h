#ifndef __GameObject_H__
#define __GameObject_H__

#include <vector>
#include <string>
#include "Globals.h"
#include "MathGeoLib-1.5/src/Geometry/AABB.h"
#include "parson/parson.h"
#include "DebugCube.h"

enum COMPONENT_TYPE
{
	COMPONENT_NONE = 0,
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_MATERIAL,
	COMPONENT_CAMERA
};

class Component;

class GameObject
{
public:
	GameObject();
	GameObject(const char* name, GameObject* parent = nullptr);
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

	void UpdateAABB();

	// Static
	bool GetIsStatic() const;
	void SetIsStatic(bool set);

	uuid_unit GetUUID() const;
	void SetUUID(uuid_unit nUUID);

	JSON_Value* OnSaveJSON();

public:
	math::AABB boundingBox;
	DebugCube* boundingBoxCube = nullptr;

private:
	GameObject* parent = nullptr;
	std::string name = "";
	bool active = true;
	bool isStatic = false;

	std::vector<GameObject*> children;
	std::vector<Component*> components;

	uuid_unit uuid = 0u;

};

#endif