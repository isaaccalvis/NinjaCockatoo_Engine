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

	// Clean last Constraint
	App->physics->DeleteConstraint(constraint);

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
				bodyApoint,
				bodyBpoint);
		}
		break;
		case PHYSIC_CONSTRAINT::CONSTRAINT_HINGE:
		{
			constraint = App->physics->CreateConstraint(
				PHYSIC_CONSTRAINT::CONSTRAINT_P2P,
				parent->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody,
				connectedGO->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody,
				bodyApoint,
				bodyBpoint,
				bodyAaxis,
				bodyBaxis);
		}
		break;
		case PHYSIC_CONSTRAINT::CONSTRAINT_SLIDER:
		{
			constraint = App->physics->CreateConstraint(
				PHYSIC_CONSTRAINT::CONSTRAINT_P2P,
				parent->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody,
				connectedGO->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody,
				bodyApoint,
				bodyBpoint);
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
	if (parent != nullptr)
	{
		json_object_set_string(object, "c_type", "c_constraint");
		json_object_set_number(object, "ConstraintUUID", constraintUUID);
		// Connected GO
		if (connectedGO != nullptr)
			json_object_set_number(object, "ConnectedGO", connectedGO->GetUUID());
		else
			json_object_set_number(object, "ConnectedGO", 0);
		json_object_set_number(object, "ConstraintType", (int)type);
		// bodyApoint
		json_object_set_number(object, "bodyApointX", bodyApoint.x);
		json_object_set_number(object, "bodyApointY", bodyApoint.y);
		json_object_set_number(object, "bodyApointZ", bodyApoint.z);
		// bodyBpoint
		json_object_set_number(object, "bodyBpointX", bodyBpoint.x);
		json_object_set_number(object, "bodyBpointY", bodyBpoint.y);
		json_object_set_number(object, "bodyBpointZ", bodyBpoint.z);
		// bodyAaxis
		json_object_set_number(object, "bodyAaxisX", bodyAaxis.x);
		json_object_set_number(object, "bodyAaxisY", bodyAaxis.y);
		json_object_set_number(object, "bodyAaxisZ", bodyAaxis.z);
		// bodyBaxis
		json_object_set_number(object, "bodyBaxisX", bodyBaxis.x);
		json_object_set_number(object, "bodyBaxisY", bodyBaxis.y);
		json_object_set_number(object, "bodyBaxisZ", bodyBaxis.z);
	}
}

void C_PhysicsConstraint::OnLoadJson(JSON_Object* object)
{
	constraintUUID = json_object_get_number(object, "ConstraintUUID");
	// Connected GO
	uuid_unit connectedGOUUID = json_object_get_number(object, "ConnectedGO");
	connectedGO = App->scene->SearchGameObject(connectedGOUUID);

	type = (PHYSIC_CONSTRAINT)((int)json_object_get_number(object, "ConstraintType"));
	// bodyApoint
	bodyApoint.x = json_object_get_number(object, "bodyApointX");
	bodyApoint.y = json_object_get_number(object, "bodyApointY");
	bodyApoint.z = json_object_get_number(object, "bodyApointZ");
	// bodyBpoint
	bodyBpoint.x = json_object_get_number(object, "bodyBpointX");
	bodyBpoint.y = json_object_get_number(object, "bodyBpointY");
	bodyBpoint.z = json_object_get_number(object, "bodyBpointZ");
	// bodyAaxis
	bodyAaxis.x = json_object_get_number(object, "bodyAaxisX");
	bodyAaxis.y = json_object_get_number(object, "bodyAaxisY");
	bodyAaxis.z = json_object_get_number(object, "bodyAaxisZ");
	// bodyBaxis
	bodyBaxis.x = json_object_get_number(object, "bodyBaxisX");
	bodyBaxis.y = json_object_get_number(object, "bodyBaxisY");
	bodyBaxis.z = json_object_get_number(object, "bodyBaxisZ");

	if (connectedGO != nullptr)
	{
		ConnectGameObject(connectedGO);
	}
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

void C_PhysicsConstraint::SetBodyApoint(math::float3 point)
{
	this->bodyApoint = point;
}

void C_PhysicsConstraint::SetBodyBpoint(math::float3 point)
{
	this->bodyBpoint = point;
}

void C_PhysicsConstraint::SetBodyAaxis(math::float3 point)
{
	this->bodyAaxis = point;
}

void C_PhysicsConstraint::SetBodyBaxis(math::float3 point)
{
	this->bodyBaxis = point;
}