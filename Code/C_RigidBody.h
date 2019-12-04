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
	
private:
	btRigidBody* rigidBody = nullptr;
	float mass = 1.0f;

};

#endif