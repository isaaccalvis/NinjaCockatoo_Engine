#ifndef __C_RigidBody_H__
#define __C_RigidBody_H__

#include "Globals.h"
#include "Component.h"

class C_RigidBody : public Component
{
public:
	C_RigidBody(GameObject* parent);
	~C_RigidBody();

	void Update(float dt);

	void SetMass(float mass);
	float GetMass() const;

	void UpdatePosition();
	void SetPosition(math::float3 position);

	void SetSize(math::float3 size);
	math::float3 GetSize() const;
	
private:
	btRigidBody* rigidBody = nullptr;
	float mass = 1.0f;
	math::float3 size = math::float3::one;
};

#endif