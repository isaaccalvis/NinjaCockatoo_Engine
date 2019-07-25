#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib-1.5\src\MathGeoLib.h"

class Camera {
public:
	Camera();

	void Init();

	float* GetViewMatrix() const;
	float* GetProjectionMatrix() const;

public:
	Frustum frustum;

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

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();
	
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const math::float3 &Position, const math::float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const math::float3 &Spot);
	void Move(const math::float3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	math::float3 X, Y, Z, Position, Reference;
	Camera camera;

private:

	math::float4x4 ViewMatrix, ViewMatrixInverse;
};