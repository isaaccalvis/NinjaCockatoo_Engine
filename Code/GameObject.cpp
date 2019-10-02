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

Component* GameObject::CreateComponent(COMPONENT_TYPE type, std::string name)
{
	return nullptr;
}

Component* GameObject::GetComponent(COMPONENT_TYPE type, std::string name)
{
	return nullptr;
}