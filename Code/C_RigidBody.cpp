#include "Application.h"
#include "C_RigidBody.h"

C_RigidBody::C_RigidBody(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_RIGIDBODY)
{
	isUnique = true;
}

C_RigidBody::~C_RigidBody()
{

}

void C_RigidBody::Update(float dt)
{

}