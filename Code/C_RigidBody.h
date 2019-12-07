#ifndef __C_RigidBody_H__
#define __C_RigidBody_H__

#include "Globals.h"
#include "Component.h"
#include "C_PhysicsConstraint.h"

class C_RigidBody : public Component
{
public:
	C_RigidBody(GameObject* parent);
	~C_RigidBody();

	void Update(float dt);

	void OnSaveJson(JSON_Object* object);
	void OnLoadJson(JSON_Object* object);

	void SetMass(float mass);
	float GetMass() const;

	void UpdatePosition();
	void SetPosition(math::float3 position);

	void SetSize(math::float3 size);
	math::float3 GetSize() const;
	
public:
	uuid_unit rigidBody_uuid = 0u;

public:
	btRigidBody* rigidBody = nullptr;
	C_PhysicsConstraint* connectedConstraint = nullptr;
	float mass = 1.0f;
	math::float3 size = math::float3::one;
};

#endif