#include <list>

#include "Application.h"
#include "C_Transform.h"
#include "C_Collider.h"
#include "C_RigidBody.h"

C_Transform::C_Transform(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_TRANSFORM)
{
	isUnique = true;
	name = "Transform";

}

C_Transform::~C_Transform() {}

void C_Transform::Update(float dt)
{
	if (parent->GetComponent(COMPONENT_RIGIDBODY) != nullptr)
	{
		const btVector3 rbPos = parent->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody->getCenterOfMassPosition();
		this->position = math::float3(rbPos.getX(), rbPos.getY(), rbPos.getZ());
		UpdateGlobalMatrix();
		UpdateGlobalMatrixOfChilds();
		parent->UpdateAABB();
	}
}

void C_Transform::OnSaveJson(JSON_Object* object)
{
	if (parent != nullptr)
	{
		json_object_set_string(object, "c_type", "c_transform");
		json_object_set_number(object, "PositionX", position.x);
		json_object_set_number(object, "PositionY", position.y);
		json_object_set_number(object, "PositionZ", position.z);

		json_object_set_number(object, "RotationX", rotation.x);
		json_object_set_number(object, "RotationY", rotation.y);
		json_object_set_number(object, "RotationZ", rotation.z);
		json_object_set_number(object, "RotationW", rotation.w);

		json_object_set_number(object, "ScaleX", scale.x);
		json_object_set_number(object, "ScaleY", scale.y);
		json_object_set_number(object, "ScaleZ", scale.z);
	}
}

void C_Transform::OnLoadJson(JSON_Object* object)
{
	position.x = json_object_get_number(object, "PositionX");
	position.y = json_object_get_number(object, "PositionY");
	position.z = json_object_get_number(object, "PositionZ");

	rotation.x = json_object_get_number(object, "RotationX");
	rotation.y = json_object_get_number(object, "RotationY");
	rotation.z = json_object_get_number(object, "RotationZ");
	rotation.w = json_object_get_number(object, "RotationW");

	scale.x = json_object_get_number(object, "ScaleX");
	scale.y = json_object_get_number(object, "ScaleY");
	scale.z = json_object_get_number(object, "ScaleZ");
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

void C_Transform::SetMatrixFromGlobal(math::float4x4& globalMatrix)
{
	math::float4x4 newMatrix = parent->GetParent()->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->GetGlobalMatrix().Inverted();
	newMatrix = newMatrix * globalMatrix;
	newMatrix.Decompose(position, rotation, scale);
	UpdateGlobalMatrix();
	parent->UpdateAABB();
	if (parent->GetComponent(COMPONENT_COLLIDER) != nullptr)
		parent->GetComponent(COMPONENT_COLLIDER)->GetComponentAsCollider()->UpdatePosition();
	if (parent->GetComponent(COMPONENT_RIGIDBODY) != nullptr)
		parent->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsCollider()->UpdatePosition();
}

void C_Transform::UpdateGlobalMatrix()
{
	globalMatrix.Set(GetGlobalMatrix());
	globalMatrix.Decompose(globalPosition, globalRotation, globalScale);
}

void C_Transform::UpdateGlobalMatrixOfChilds()
{
	for (int i = 0; i < parent->CountChild(); i++)
	{
		parent->GetChild(i)->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
		parent->GetChild(i)->UpdateAABB();
		if (parent[i].CountChild() > 0)
		{
			parent->GetChild(i)->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
		}
	}
}