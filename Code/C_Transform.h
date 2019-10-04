#ifndef __C_Transform_H__
#define __C_Transform_H__

#include "Component.h"

#include "MathGeoLib-1.5/src/Math/float3.h"
#include "MathGeoLib-1.5/src/Math/Quat.h"
#include "MathGeoLib-1.5/src/Math/float4x4.h"

class C_Transform : public Component
{
public:
	C_Transform(GameObject* parent);
	~C_Transform();

	void Update(float dt);

	math::float4x4& GetMatrix() const;

public:
	math::float3 position = math::float3::zero;
	math::Quat rotation = math::Quat::identity;
	math::float3 scale = math::float3::one;
};

#endif