#ifndef __C_PhysicsConstraint_H__
#define __C_PhysicsConstraint_H__

#include "Globals.h"
#include "Component.h"

class C_PhysicsConstraint : public Component
{
public:
	C_PhysicsConstraint(GameObject* parent);
	~C_PhysicsConstraint();

	bool ConnectRigidBody(GameObject* go);

	bool GenerateConstraint();

	void Update(float dt);

	void OnSaveJson(JSON_Object* object);
	void OnLoadJson(JSON_Object* object);

public:
	btTypedConstraint* constraint = nullptr;
	GameObject* connectedGO = nullptr;
};

#endif