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

	void				SetMass(float mass);
	float				GetMass() const;

	void				SetGravity(bool set);
	bool				GetGravity() const;

	void				UpdatePosition();
	void				SetPosition(math::float3 position);

	void				SetSize(math::float3 size);
	math::float3		GetSize() const;

	const char*			GetShapeString();
	PHYSIC_PRIMITIVE	GetShape();
	void				SetShape(PHYSIC_PRIMITIVE primitive);
	void				SetShape(const char* primitiveName);

	void				ClearForces();
	
	void				AddCollidingGameObjectList(GameObject* go);
	void				ClearCollidingGameObjectList();

public:
	uuid_unit rigidBody_uuid = 0u;

	btRigidBody* rigidBody = nullptr;

	math::float3 size = math::float3::one;
	float mass = 1.0f;

	PHYSIC_PRIMITIVE shapePrimitive;
	bool useGravity = true;

	C_PhysicsConstraint* connectedConstraint = nullptr;

	std::list<GameObject*> collidingGameObjects;
};

#endif