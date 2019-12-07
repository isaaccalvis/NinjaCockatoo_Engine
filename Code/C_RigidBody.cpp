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
	rigidBody = App->physics->CreateRigidBody(PHY_CUBE,math::float3(size.x, size.y, size.z), mass);

	UpdatePosition();
}

C_RigidBody::~C_RigidBody()
{
	App->physics->DeleteRigidBody(rigidBody);
	if (connectedConstraint != nullptr)
	{
		connectedConstraint->connectedGO = nullptr;
		App->physics->DeleteConstraint(connectedConstraint->constraint);
	}
}

void C_RigidBody::Update(float dt)
{

}

void C_RigidBody::OnSaveJson(JSON_Object* object)
{
	if (parent != nullptr)
	{
		json_object_set_string(object, "c_type", "c_rigidbody");
		json_object_set_number(object, "RigidBodyUUID", rigidBody_uuid);
		json_object_set_number(object, "SizeX", size.x);
		json_object_set_number(object, "SizeY", size.y);
		json_object_set_number(object, "SizeZ", size.z);
		json_object_set_number(object, "Mass", mass);
	}
}

void C_RigidBody::OnLoadJson(JSON_Object* object)
{
	rigidBody_uuid = json_object_get_number(object, "RigidBodyUUID");
	size.x = json_object_get_number(object, "SizeX");
	size.y = json_object_get_number(object, "SizeY");
	size.z = json_object_get_number(object, "SizeZ");
	mass = json_object_get_number(object, "Mass");
}

void C_RigidBody::SetMass(float mass)
{
	this->mass = mass;
	App->physics->physicsWorld->removeRigidBody(rigidBody);
	rigidBody->getCollisionShape()->calculateLocalInertia(mass, btVector3(0,0,0));
	rigidBody->setMassProps(mass, btVector3(0,0,0));
	rigidBody->clearForces();
	App->physics->physicsWorld->addRigidBody(rigidBody);
	rigidBody->activate(true);
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