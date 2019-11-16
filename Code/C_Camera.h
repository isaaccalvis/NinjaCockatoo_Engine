#ifndef __C_Camera_H__
#define __C_Camera_H__

#include "Component.h"
#include "MathGeoLib-1.5/src/Geometry/Frustum.h"

class C_Camera : public Component
{
public:
	C_Camera(GameObject* parent);
	~C_Camera();

	void Update(float dt);

	void OnSaveJson(JSON_Object* object);

	void InitFrustum();
	void RenderCamera();
	void UpdateTransform();
	void BecomeMainCamera();
	bool IsInsideFrustumCulling(GameObject* go);

public:
	Frustum frustum;
	DebugCube* debugCube = nullptr;
	bool renderCameraFrustum = true;
	bool isMainCamera = false;
	bool frustumCulling = false;

private:
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