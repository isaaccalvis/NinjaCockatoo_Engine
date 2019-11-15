#include <algorithm>
#include "Application.h"
#include "GameObject.h"

#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Camera.h"

GameObject::GameObject()
{
	uuid = App->input->GenerateUUID();
	CreateComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM);
	boundingBox.SetNegativeInfinity();
}

GameObject::GameObject(const char* name, GameObject* parent)
{
	uuid = App->input->GenerateUUID();
	this->name = name;
	if (parent != nullptr)
	{
		this->parent = parent;
		this->parent->children.push_back(this);
	}
	// Component transform is always necessary
	CreateComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM);
	boundingBox.SetNegativeInfinity();
}

GameObject::~GameObject()
{
	// Clean goSelected
	if (this == App->scene->goSelected)
		App->scene->goSelected = nullptr;

	// Delete Components
	for (int i = 0; i < components.size(); i++)
	{
		delete components[i];
		//components.erase(components.begin() + i);
	}
	components.clear();

	// Delete Children
	for (int i = children.size() - 1; i >= 0 ; i--)
	{
		
		// TODO: use this to clean all vectors
		//children.erase(std::remove(children.begin(), children.end(), children[i]), children.end());
		delete children[i];
		//children.erase(children.begin() + i);
	}
	children.clear();

	// Delete from parent vector
	if (parent != nullptr)
	{
		parent->QuitChildFromVector(this);
	}
}

const char* GameObject::GetName() const
{
	return name.c_str();
}

void GameObject::SetName(const char* name)
{
	this->name = name;
}

bool GameObject::IsActive() const
{
	return active;
}

void GameObject::SetActive(bool set)
{
	this->active = set;
}

Component* GameObject::CreateComponent(COMPONENT_TYPE type, const char* name)
{
	// See if exist an unique component
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type && components[i]->isUnique == true)
		{
			LOG_CONSOLE("Can't add component, it already exist and is unique for gameobject");
			return nullptr;
		}
	}

	Component* component = nullptr;
	switch (type)
	{
	case COMPONENT_TYPE::COMPONENT_TRANSFORM:
	{
		component = new C_Transform(this);
	}
	break;
	case COMPONENT_TYPE::COMPONENT_MESH:
	{
		component = new C_Mesh(this);
	}
	break;
	case COMPONENT_TYPE::COMPONENT_MATERIAL:
	{
		component = new C_Material(this);
	}
	break;
	case COMPONENT_TYPE::COMPONENT_CAMERA:
	{
		component = new C_Camera(this);
	}
	break;
	}
	component->parent = this;
	components.push_back(component);
	return component;
}

Component* GameObject::GetComponent(COMPONENT_TYPE type, const char* name)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type)
			return components[i];
	}
	return nullptr;
}

Component* GameObject::GetComponent(int num) const
{
	return components[num];
}

void GameObject::DeleteComponent(COMPONENT_TYPE type, const char* name)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type)
		{
			if (name != nullptr)
			{
				if (components[i]->name == name)
				{
					delete components[i];
					components.erase(components.begin() + i);
				}
			}
			else
			{
				delete components[i];
				components.erase(components.begin() + i);
			}
		}
	}
}

int GameObject::CountComponents() const
{
	return components.size();
}

void GameObject::UpdateAABB()
{
	if (GetComponent(COMPONENT_MESH) != nullptr)
	{
		boundingBox.SetNegativeInfinity();
		boundingBox.SetFrom(&GetComponent(COMPONENT_MESH)->GetComponentAsMesh()->GetMesh()->vectorVertex[0], GetComponent(COMPONENT_MESH)->GetComponentAsMesh()->GetMesh()->vectorVertex.size());

		math::OBB obb = boundingBox;
		obb.Transform(GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalMatrix);
		boundingBox = obb.MinimalEnclosingAABB();

		if (boundingBoxCube != nullptr)
		{
			boundingBoxCube->position = boundingBox.CenterPoint();
			boundingBoxCube->scale = boundingBox.Size();
		}
		else
		{
			boundingBoxCube = new DebugCube(boundingBox.CenterPoint(), boundingBox.Size());
		}
	}
	else
	{
		boundingBox.SetNegativeInfinity();
		if (boundingBoxCube != nullptr)
		{
			delete boundingBoxCube;
			boundingBoxCube = nullptr;
		}
	}
}

bool GameObject::GetIsStatic() const
{
	return isStatic;
}

void GameObject::SetIsStatic(bool set)
{
	isStatic = set;
	// Add at quadtree
	if (App->scene->quadTree != nullptr)
	{
		if (isStatic)
		{
			App->scene->quadTree->Insert(this);
		}
		else
		{
			App->scene->quadTree->Remove(this);
		}
	}
}

uuid_unit GameObject::GetUUID() const
{
	return uuid;
}

void GameObject::SetUUID(uuid_unit nUUID)
{
	this->uuid = nUUID;
}

GameObject* GameObject::GetParent() const
{
	return parent;
}

void GameObject::AddChildren(GameObject* child)
{
	if (child != nullptr)
	{
		children.push_back(child);
	}
}

void GameObject::SetParent(GameObject* parent)
{
	this->parent = parent;
}

void GameObject::DeleteChild(GameObject* child)
{
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i] == child)
		{
			delete children[i];
			children.erase(children.begin() + i);
		}
	}
}

void GameObject::QuitChildFromVector(GameObject* child)
{
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i] == child)
		{
			children.erase(children.begin() + i);
		}
	}
}

GameObject* GameObject::GetChild(int num) const
{
	return children[num];
}

int GameObject::CountChild()
{
	return children.size();
}

void GameObject::OnSaveRecursiveJson(JSON_Array* array)
{
	JSON_Value* value = json_value_init_object();
	JSON_Object* object = json_value_get_object(value);

	json_object_set_string(object, "Name", name.c_str());
	json_object_set_number(object, "UUID", uuid);
	if (parent != nullptr)
		json_object_set_number(object, "Parent UUID", parent->uuid);
	else
		json_object_set_number(object, "Parent UUID", 0);
	json_array_append_value(array, value);

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->OnSaveRecursiveJson(array);
	}
}