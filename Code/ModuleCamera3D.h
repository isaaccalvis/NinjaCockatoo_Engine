#pragma once
#include "Module.h"
#include "Globals.h"
#include "C_Transform.h"
#include "Component.h"
#include "GameObject.h"
#include "MathGeoLib-1.5\src\MathGeoLib.h"

class Camera {
public:
	Camera();

	void Init();

	math::float4x4		GetViewMatrix() const;
	math::float4x4		GetProjectionMatrix() const;

public:
	math::Frustum	frustum;

private:
	bool			culling = false;
	bool			cull_dynamics = true;
	bool			is_main = false;
	float			width = 0.0f;
	float			height = 0.0f;
	float			aspect_ratio = 0.0f;
	float			near_plane = 0.0f;
	float			far_plane = 0.0f;
	float			vertical_fov = 0.0f;
	float			frustum_halfdistance_squared = 0;
	math::float3	frustum_center = math::float3::zero;
};

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();
	
	update_status	Update(float dt);
	bool			CleanUp();

	bool			Save(JSON_Object* root_object);
	bool			Load(JSON_Object* root_object);

	void			LookAt(const math::float3 &Spot, float distance = 0.0f);
	void			Orbit(math::float3 target, float deltaX, float deltaY);

public:
	Camera		camera;
	Camera		editorCameraCopy;
	bool		isCameraEditor = true;
	float		camera_mov_speed = 3.0f;
	float		mouse_wheel_speed = 0.5f;
	float		mouse_sensitivity = 0.25f;
};