#include <list>

#include "C_Transform.h"

C_Transform::C_Transform(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_TRANSFORM)
{
	isUnique = true;
}

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

math::float4x4 C_Transform::GetGlobalMatrix() const
{
	math::float4x4 localMatrix = GetMatrix();

	if (parent->GetParent() != nullptr)
	{
		math::float4x4 tmpGlobalMatrix = math::float4x4::identity;
		tmpGlobalMatrix = parent->GetParent()->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->GetGlobalMatrix();
		return tmpGlobalMatrix * localMatrix;
	}

	return localMatrix;
}

void C_Transform::UpdateGlobalMatrix()
{
	globalMatrix.Set(GetGlobalMatrix());
}

void C_Transform::UpdateGlobalMatrixOfChilds()
{
	for (int i = 0; i < parent->CountChild(); i++)
	{
		parent->GetChild(i)->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
	}
}