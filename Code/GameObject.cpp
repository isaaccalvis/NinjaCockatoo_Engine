#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::GameObject(std::string name, GameObject* parent)
{
	this->name = name;
	this->parent = parent;
}

GameObject::~GameObject()
{

}

void GameObject::Update(float dt)
{

}

void GameObject::SetName(const char* name)
{
	this->name = name;
}

void GameObject::SetParent(GameObject* parent)
{
	this->parent = parent;
}

std::string GameObject::GetName() const
{
	return name;
}

GameObject* GameObject::GetParent() const
{
	return parent;
}

Component* GameObject::CreateComponent(COMPONENT_TYPE type, std::string name)
{
	return nullptr;
}

Component* GameObject::GetComponent(COMPONENT_TYPE type, std::string name)
{
	return nullptr;
}