#include "C_Transform.h"

C_Transform::C_Transform(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_TRANSFORM)
{}

C_Transform::~C_Transform()
{

}

void C_Transform::Update(float dt)
{

}

math::float4x4& C_Transform::GetMatrix() const
{
	return math::float4x4::FromTRS(position, rotation, scale);
}