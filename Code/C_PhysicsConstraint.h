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

public:
	btTypedConstraint* constraint = nullptr;
	GameObject* connectedGO = nullptr;
	bool tryingConnect = false;
	PHYSIC_CONSTRAINT type = PHYSIC_CONSTRAINT::CONSTRAINT_P2P;
};

#endif