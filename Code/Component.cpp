#include "Component.h"

Component::Component(GameObject* parent, COMPONENT_TYPE type)
{
	this->parent = parent;
	this->type = type;
}

Component::~Component()
{

}

void Component::SetEnable(bool set)
{
	active = set;
}

bool Component::GetEnabled()
{
	return active;
}

C_Transform* Component::GetComponentTransform() const
{
	return (C_Transform*)this;
}

C_Mesh* Component::GetComponentMesh() const
{
	return (C_Mesh*)this;
}

C_Material* Component::GetComponentMaterial() const
{
	return (C_Material*)this;
}