#ifndef __C_Collider_H__
#define __C_Collider_H__

#include "Globals.h"
#include "Component.h"

class C_Collider : public Component
{
public:
	C_Collider(GameObject* parent);
	~C_Collider();

	void Update(float dt);

	void UpdatePosition();
	void SetPosition(math::float3 position);

	void SetSize(math::float3 size);
	math::float3 GetSize() const;

private:
	btRigidBody* rigidBody = nullptr;
	math::float3 size = math::float3::one;

};

#endif