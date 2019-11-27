#include "Application.h"
#include "C_Collider.h"

C_Collider::C_Collider(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_COLLIDER)
{
	isUnique = true;
}

C_Collider::~C_Collider()
{

}

void C_Collider::Update(float dt)
{

}