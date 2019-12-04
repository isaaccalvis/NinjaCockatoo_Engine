#include "Application.h"
#include "C_Collider.h"
#include "C_Mesh.h"

C_Collider::C_Collider(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_COLLIDER)
{
	isUnique = true;
	name = "Collider";

	// Check if GameObject have mesh
	if (parent->GetComponent(COMPONENT_MESH) != nullptr)
	{
		size.x = parent->boundingBox.MaxX() - parent->boundingBox.MinX();
		size.y = parent->boundingBox.MaxY() - parent->boundingBox.MinY();
		size.z = parent->boundingBox.MaxZ() - parent->boundingBox.MinZ();
	}
	rigidBody = App->physics->CreateRigidBody(math::float3(size.x, size.y, size.z), 0);

	UpdatePosition();
}

C_Collider::~C_Collider()
{

}

void C_Collider::Update(float dt)
{

}

void C_Collider::UpdatePosition()
{
	math::float3 goPosition = parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition;
	SetPosition(goPosition);
}

void C_Collider::SetPosition(math::float3 position)
{
	btTransform bulletTransform = btTransform::getIdentity();
	bulletTransform.setOrigin(btVector3(position.x, position.y, position.z));
	rigidBody->setCenterOfMassTransform(bulletTransform);
}

void C_Collider::SetSize(math::float3 size)
{
	this->size = size;
	rigidBody->getCollisionShape()->setLocalScaling(btVector3(
		size.x, size.y, size.z));
}

math::float3 C_Collider::GetSize() const
{
	return size;
}