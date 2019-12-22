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
	rigidBody = App->physics->CreateRigidBody(PHY_CUBE, math::float3(size.x, size.y, size.z), 0);
	shapePrimitive = PHY_CUBE;
	UpdatePosition();
}

C_Collider::~C_Collider()
{
	if (rigidBody->getCollisionShape() != nullptr)
	{
		App->physics->DeleteShape(rigidBody->getCollisionShape());
	}

	if (rigidBody != nullptr)
	{
		App->physics->DeleteRigidBody(rigidBody);
	}
}

void C_Collider::Update(float dt)
{
	//rigidBody->checkCollideWith();
}

void C_Collider::OnSaveJson(JSON_Object* object)
{
	if (parent != nullptr)
	{
		json_object_set_string(object, "c_type", "c_collider");
		json_object_set_number(object, "ColliderUUID", colliderUUID);
		json_object_set_number(object, "SizeX", size.x);
		json_object_set_number(object, "SizeY", size.y);
		json_object_set_number(object, "SizeZ", size.z);
		json_object_set_number(object, "LocalX", localPosition.x);
		json_object_set_number(object, "LocalY", localPosition.y);
		json_object_set_number(object, "LocalZ", localPosition.z);
		json_object_set_number(object, "Shape", (int)shapePrimitive);
		json_object_set_boolean(object, "IsTrigger", isTrigger);
	}
}

void C_Collider::OnLoadJson(JSON_Object* object)
{
	colliderUUID =		json_object_get_number(object, "ColliderUUID");
	size.x =			json_object_get_number(object, "SizeX");
	size.y =			json_object_get_number(object, "SizeY");
	size.z =			json_object_get_number(object, "SizeZ");
	localPosition.x =	json_object_get_number(object, "LocalX");
	localPosition.y =	json_object_get_number(object, "LocalY");
	localPosition.z =	json_object_get_number(object, "LocalZ");
	shapePrimitive =	(PHYSIC_PRIMITIVE)((int)json_object_get_number(object, "Shape"));
	isTrigger =			json_object_get_boolean(object, "IsTrigger");

	UpdatePosition();
	SetSize(size);
	SetLocalPosition(localPosition);
	SetShape(shapePrimitive);
	SetIsTrigger(isTrigger);
}

void C_Collider::UpdatePosition()
{
	math::float3 goPosition = parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition;
	goPosition += localPosition;
	SetPosition(goPosition);
}

void C_Collider::SetPosition(math::float3 position)
{
	btTransform bulletTransform = btTransform::getIdentity();
	bulletTransform.setOrigin(btVector3(position.x, position.y, position.z));
	rigidBody->setCenterOfMassTransform(bulletTransform);
}

math::float3 C_Collider::GetLocalPosition()
{
	return localPosition;
}

void C_Collider::SetLocalPosition(math::float3 position)
{
	this->localPosition = position;
	UpdatePosition();
}

const char* C_Collider::GetShapeString()
{
	switch (shapePrimitive)
	{
	case PHYSIC_PRIMITIVE::PHY_NONE:
		return "None";
		break;
	case PHYSIC_PRIMITIVE::PHY_CUBE:
		return "Cube";
		break;
	case PHYSIC_PRIMITIVE::PHY_SPHERE:
		return "Sphere";
		break;
	}
}

PHYSIC_PRIMITIVE C_Collider::GetShape()
{
	return shapePrimitive;
}

void C_Collider::SetShape(PHYSIC_PRIMITIVE primitive)
{
	// Clear old shape
	if (rigidBody->getCollisionShape() != nullptr)
	{
		App->physics->DeleteShape(rigidBody->getCollisionShape());
	}

	shapePrimitive = primitive;
	// Actualitzar el rigidbody
	btCollisionShape* shape;
	switch (shapePrimitive)
	{
	case PHYSIC_PRIMITIVE::PHY_NONE:
		shape = new btEmptyShape();
		break;
	case PHYSIC_PRIMITIVE::PHY_CUBE:
		shape = new btBoxShape(btVector3(size.x / 2, size.y / 2, size.z / 2));
		break;
	case PHYSIC_PRIMITIVE::PHY_SPHERE:
		shape = new btSphereShape(size.x / 2);
		break;
	}
	rigidBody->setCollisionShape(shape);
}

void C_Collider::SetShape(const char* primitiveName)
{
	// Clear old shape
	if (rigidBody->getCollisionShape() != nullptr)
	{
		App->physics->DeleteShape(rigidBody->getCollisionShape());
	}

	std::string primitiveNameString(primitiveName);
	btCollisionShape* shape;
	if (primitiveNameString.compare("None") == 0)
	{
		shapePrimitive = PHYSIC_PRIMITIVE::PHY_NONE;
		shape = new btEmptyShape();
	}
	else if (primitiveNameString.compare("Cube") == 0)
	{
		shapePrimitive = PHYSIC_PRIMITIVE::PHY_CUBE;
		shape = new btBoxShape(btVector3(size.x / 2, size.y / 2, size.z / 2));
	}
	else if (primitiveNameString.compare("Sphere") == 0)
	{
		shapePrimitive = PHYSIC_PRIMITIVE::PHY_SPHERE;
		shape = new btSphereShape(size.x / 2);
	}
	// Actualitzar el rigidbody
	rigidBody->setCollisionShape(shape);
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

bool C_Collider::GetIsTrigger() const
{
	return isTrigger;
}

void C_Collider::SetIsTrigger(bool set)
{
	isTrigger = set;
	if (set)
	{
		rigidBody->setCollisionFlags(4);
	}
	else
	{
		rigidBody->setCollisionFlags(1);
	}
}

void C_Collider::AddCollidingGameObjectList(GameObject* go)
{
	if (go != nullptr)
	{
		collidingGameObjects.push_back(go);
	}
}

void C_Collider::ClearCollidingGameObjectList()
{
	collidingGameObjects.clear();
}