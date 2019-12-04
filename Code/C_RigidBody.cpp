#include "Application.h"
#include "C_RigidBody.h"

C_RigidBody::C_RigidBody(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_RIGIDBODY)
{
	isUnique = true;
	name = "RigidBody";

	// Check if GameObject have mesh
	if (parent->GetComponent(COMPONENT_MESH) != nullptr)
	{
		size.x = parent->boundingBox.MaxX() - parent->boundingBox.MinX();
		size.y = parent->boundingBox.MaxY() - parent->boundingBox.MinY();
		size.z = parent->boundingBox.MaxZ() - parent->boundingBox.MinZ();
	}
	rigidBody = App->physics->CreateRigidBody(math::float3(size.x, size.y, size.z), mass);

	UpdatePosition();
}

C_RigidBody::~C_RigidBody()
{

}

void C_RigidBody::Update(float dt)
{

}

void C_RigidBody::SetMass(float mass)
{
	this->mass = mass;
	rigidBody->setMassProps(mass, btVector3(0,0,0));
}

float C_RigidBody::GetMass() const
{
	return mass;
}

void C_RigidBody::UpdatePosition()
{
	math::float3 goPosition = parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition;
	btTransform bulletTransform = btTransform::getIdentity();
	bulletTransform.setOrigin(btVector3(goPosition.x, goPosition.y, goPosition.z));
	rigidBody->setCenterOfMassTransform(bulletTransform);
}

void C_RigidBody::SetPosition(math::float3 position)
{
	btTransform bulletTransform = btTransform::getIdentity();
	bulletTransform.setOrigin(btVector3(position.x, position.y, position.z));
	rigidBody->setCenterOfMassTransform(bulletTransform);
}

void C_RigidBody::SetSize(math::float3 size)
{
	this->size = size;
	rigidBody->getCollisionShape()->setLocalScaling(btVector3(
		size.x, size.y, size.z));
}

math::float3 C_RigidBody::GetSize() const
{
	return size;
}