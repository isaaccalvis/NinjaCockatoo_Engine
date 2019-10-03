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