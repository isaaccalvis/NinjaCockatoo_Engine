#ifndef __C_Camera_H__
#define __C_Camera_H__

#include "Component.h"
#include "MathGeoLib-1.5/src/Geometry/Frustum.h"

class C_Camera : public Component
{
public:
	C_Camera(GameObject* parent);
	~C_Camera();

	void InitFrustum();
	void RenderCamera();

	void Update(float dt);

public:
	Frustum frustum;
	bool renderCameraFrustum = true;
	DebugCube* debugCube = nullptr;

private:
	bool culling = false;
	bool cull_dynamics = true;
	bool is_main = false;
	float width = 0.0f;
	float height = 0.0f;
	float aspect_ratio = 0.0f;
	float near_plane = 0.0f;
	float far_plane = 0.0f;
	float vertical_fov = 0.0f;
	float frustum_halfdistance_squared = 0;
	math::float3 frustum_center = math::float3::zero;
};

#endif