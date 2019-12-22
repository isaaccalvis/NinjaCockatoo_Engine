#ifndef __C_PhysicsConstraint_H__
#define __C_PhysicsConstraint_H__

#include "Globals.h"
#include "Component.h"

class C_PhysicsConstraint : public Component
{
public:
	C_PhysicsConstraint(GameObject* parent);
	~C_PhysicsConstraint();

	bool ConnectGameObject(GameObject* go);

	bool GenerateConstraint();

	void Update(float dt);

	void OnSaveJson(JSON_Object* object);
	void OnLoadJson(JSON_Object* object);

	void SetConstraint(const char* type);

	void SetBodyApoint(math::float3 point);
	void SetBodyBpoint(math::float3 point);
	void SetBodyAaxis(math::float3 point);
	void SetBodyBaxis(math::float3 point);

public:
	btTypedConstraint* constraint = nullptr;
	uuid_unit constraintUUID = 0u;
	GameObject* connectedGO = nullptr;
	bool tryingConnect = false;
	PHYSIC_CONSTRAINT type = PHYSIC_CONSTRAINT::CONSTRAINT_P2P;
	math::float3 bodyApoint = math::float3::zero;
	math::float3 bodyBpoint = math::float3::zero;
	math::float3 bodyAaxis = math::float3::zero;
	math::float3 bodyBaxis = math::float3::zero;
};

#endif