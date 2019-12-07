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
	if (constraint != nullptr)
	{
		App->physics->DeleteConstraint(constraint);
	}
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
		switch (type)
		{
		case PHYSIC_CONSTRAINT::CONSTRAINT_P2P:
		{
			constraint = App->physics->CreateConstraint(
				PHYSIC_CONSTRAINT::CONSTRAINT_P2P,
				parent->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody,
				connectedGO->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody,
				parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition,
				connectedGO->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition);
		}
		break;
		case PHYSIC_CONSTRAINT::CONSTRAINT_HINGE:
		{
			constraint = App->physics->CreateConstraint(
				PHYSIC_CONSTRAINT::CONSTRAINT_P2P,
				parent->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody,
				connectedGO->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody,
				parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition,
				connectedGO->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition);
		}
		break;
		case PHYSIC_CONSTRAINT::CONSTRAINT_SLIDER:
		{
			constraint = App->physics->CreateConstraint(
				PHYSIC_CONSTRAINT::CONSTRAINT_P2P,
				parent->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody,
				connectedGO->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody,
				parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition,
				connectedGO->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition);
		}
		break;
		}
	}

	if (constraint != nullptr)
	{
		parent->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->connectedConstraint = this;
		connectedGO->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->connectedConstraint = this;
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

void C_PhysicsConstraint::SetConstraint(const char* type)
{
	if (constraint != nullptr)
	{
		App->physics->DeleteConstraint(constraint);
		constraint = nullptr;
	}

	std::string constraintType(type);
	if (constraintType.compare("Point2Point") == 0)
	{
		this->type = PHYSIC_CONSTRAINT::CONSTRAINT_P2P;
	}
	else if (constraintType.compare("Hinge") == 0)
	{
		this->type = PHYSIC_CONSTRAINT::CONSTRAINT_HINGE;
	}
	else if (constraintType.compare("Slider") == 0)
	{
		this->type = PHYSIC_CONSTRAINT::CONSTRAINT_SLIDER;
	}
}