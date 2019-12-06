#include "Application.h"

#include "C_PhysicsConstraint.h"

C_PhysicsConstraint::C_PhysicsConstraint(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_CONSTRAINT)
{
	isUnique = true;
	name = "PhysicsConstraint";
}

C_PhysicsConstraint::~C_PhysicsConstraint()
{

}

void C_PhysicsConstraint::Update(float dt)
{

}

void C_PhysicsConstraint::OnSaveJson(JSON_Object* object)
{

}

void C_PhysicsConstraint::OnLoadJson(JSON_Object* object)
{

}