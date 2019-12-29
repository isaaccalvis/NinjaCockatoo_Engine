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
	shapePrimitive = PHY_CUBE;
	UpdatePosition();
}

C_RigidBody::~C_RigidBody()
{
	App->physics->DeleteRigidBody(rigidBody);
	if (connectedConstraint != nullptr && connectedConstraint->connectedGO != nullptr)
	{
		connectedConstraint->connectedGO = nullptr;
		App->physics->DeleteConstraint(connectedConstraint->constraint);
	}
}

void C_RigidBody::Update(float dt)
{
	if (!rigidBody->isActive())
	{
		rigidBody->activate();
		rigidBody->clearForces();
	}
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
		json_object_set_number(object, "Shape", (int)shapePrimitive);
		json_object_set_boolean(object, "UseGravity", useGravity);
	}
}

void C_RigidBody::OnLoadJson(JSON_Object* object)
{
	rigidBody_uuid = json_object_get_number(object, "RigidBodyUUID");
	size.x = json_object_get_number(object, "SizeX");
	size.y = json_object_get_number(object, "SizeY");
	size.z = json_object_get_number(object, "SizeZ");
	mass = json_object_get_number(object, "Mass");
	shapePrimitive = (PHYSIC_PRIMITIVE)((int)json_object_get_number(object, "Shape"));
	useGravity = json_object_get_boolean(object, "UseGravity");

	SetShape(shapePrimitive);
	SetSize(size);
	SetMass(mass);
	SetGravity(useGravity);
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

void C_RigidBody::SetGravity(bool set)
{
	useGravity = set;
	if (set)
	{
		rigidBody->setGravity(App->physics->gravity);
	}
	else
	{
		rigidBody->setGravity(btVector3(0, 0, 0));
	}
	rigidBody->clearForces();
}

bool C_RigidBody::GetGravity() const
{
	return useGravity;
}

void C_RigidBody::UpdatePosition()
{
	math::float3 goPosition = parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition;
	btTransform bulletTransform = btTransform::getIdentity();
	bulletTransform.setOrigin(btVector3(goPosition.x, goPosition.y, goPosition.z));
	math::Quat goRotation = parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalRotation;
	bulletTransform.setRotation(btQuaternion(goRotation.x, goRotation.y, goRotation.z, goRotation.w));
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
	//rigidBody->getCollisionShape()->setLocalScaling(btVector3(
	//	size.x, size.y, size.z));
	SetShape(shapePrimitive);
}

math::float3 C_RigidBody::GetSize() const
{
	return size;
}

const char* C_RigidBody::GetShapeString()
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

PHYSIC_PRIMITIVE C_RigidBody::GetShape()
{
	return shapePrimitive;
}

void C_RigidBody::SetShape(PHYSIC_PRIMITIVE primitive)
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

void C_RigidBody::SetShape(const char* primitiveName)
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

void C_RigidBody::ClearForces()
{
	rigidBody->setLinearVelocity(btVector3(0,0,0));
	rigidBody->setAngularVelocity(btVector3(0, 0, 0));
	SetMass(mass);
	SetGravity(useGravity);
	rigidBody->clearForces();
}

void C_RigidBody::AddCollidingGameObjectList(GameObject* go)
{
	if (go != nullptr)
	{
		collidingGameObjects.push_back(go);
	}
}

void C_RigidBody::ClearCollidingGameObjectList()
{
	collidingGameObjects.clear();
}