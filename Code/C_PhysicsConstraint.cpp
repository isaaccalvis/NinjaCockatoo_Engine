#include "Application.h"

#include "C_PhysicsConstraint.h"
#include "C_RigidBody.h"

C_PhysicsConstraint::C_PhysicsConstraint(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_CONSTRAINT)
{
	isUnique = true;
	name = "PhysicsConstraint";
}

C_PhysicsConstraint::~C_PhysicsConstraint()
{

}

bool C_PhysicsConstraint::ConnectGameObject(GameObject* go)
{
	bool ret = false;
	if (go->GetComponent(COMPONENT_RIGIDBODY) != nullptr)
	{
		btRigidBody* tmpRB = go->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody;
		if (tmpRB != nullptr)
		{
			connectedGO = go;
			GenerateConstraint();
			ret = true;
		}
	}
	if (ret == false)
	{
		LOG_CONSOLE("Cant connect Game Object (not rigidbody found)");
	}
	return ret;
}

bool C_PhysicsConstraint::GenerateConstraint()
{
	bool ret = false;
	if (connectedGO != nullptr)
	{
		constraint = App->physics->CreateConstraint(
			PHYSIC_CONSTRAINT::CONSTRAINT_P2P,
			parent->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody,
			connectedGO->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody,
			parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition,
			connectedGO->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition);
	}
	return ret;
}

void C_PhysicsConstraint::Update(float dt)
{
	if (tryingConnect)
	{
		if (App->scene->goSelected != parent)
		{
			ConnectGameObject(App->scene->goSelected);
			tryingConnect = false;
		}
	}
}

void C_PhysicsConstraint::OnSaveJson(JSON_Object* object)
{

}

void C_PhysicsConstraint::OnLoadJson(JSON_Object* object)
{

}