#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleCamera3D";

	Position = math::float3(0.0f, 3.0f, 3.0f);
	Reference = math::float3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::CleanUp()
{
	LOG_IDE("Cleaning camera");
	return true;
}

bool ModuleCamera3D::Save(JSON_Object* root_object)
{
	json_object_set_number(root_object, "camera_movspeed", camera_mov_speed);
	json_object_set_number(root_object, "mouse_wheel_speed", mouse_wheel_speed);
	json_object_set_number(root_object, "mouse_sensitive", mouse_sensitivity);
	return true;
}

bool ModuleCamera3D::Load(JSON_Object* root_object)
{
	camera_mov_speed = (float)json_object_get_number(root_object, "camera_movspeed");
	mouse_wheel_speed = (float)json_object_get_number(root_object, "mouse_wheel_speed");
	mouse_sensitivity = (float)json_object_get_number(root_object, "mouse_sensitive");
	return true;
}

update_status ModuleCamera3D::Update(float dt)
{
	math::float3 X = math::float3(1.0f, 0.0f, 0.0f);
	math::float3 Y = math::float3(0.0f, 1.0f, 0.0f);
	math::float3 Z = math::float3(0.0f, 0.0f, 1.0f);
	math::float3 newPos(0,0,0);

	if (App->input->GetMouseZ() != 0)
	{
		camera.frustum.Translate(App->input->GetMouseZ() * camera.frustum.front * mouse_wheel_speed);
		Reference += App->input->GetMouseZ() * camera.frustum.front * mouse_wheel_speed;
	}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		float speed = camera_mov_speed * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed *= 2;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += camera.frustum.up * speed;
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos -= camera.frustum.up *speed;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += camera.frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= camera.frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= camera.frustum.WorldRight() * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += camera.frustum.WorldRight() * speed;
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
			if (App->scene->goSelected != nullptr)
				App->scene->MakeCameraLookThisGOSelected();
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->scene->goSelected != nullptr)
		{
			// Look Around (target position)
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float deltaX = (float)dx * mouse_sensitivity * dt;
			float deltaY = (float)dy * mouse_sensitivity * dt;
			Orbit(App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position, deltaY, deltaX);
		}
	}
	else if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		if(dx != 0 || dy != 0)
		{
			math::Quat rotationX = math::Quat::RotateAxisAngle({ 0.0f,1.0f,0.0f }, dx * DEGTORAD * mouse_sensitivity);
			math::Quat rotationY = math::Quat::RotateAxisAngle(camera.frustum.WorldRight(), dy * DEGTORAD * mouse_sensitivity);
			math::Quat finalRotation = rotationX * rotationY;

			camera.frustum.up = finalRotation * camera.frustum.up;
			camera.frustum.front = finalRotation * camera.frustum.front;

			float distance = (camera.frustum.pos - Reference).Length();
			camera.frustum.pos = Reference + (-camera.frustum.front * distance);
		}
	}

	Reference += newPos;
	camera.frustum.Translate(newPos);

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::LookAt( const math::float3 &Spot, float distance)
{
	math::float3 X = math::float3(1.0f, 0.0f, 0.0f);
	math::float3 Y = math::float3(0.0f, 1.0f, 0.0f);
	math::float3 Z = math::float3(0.0f, 0.0f, 1.0f);

	Z = -(camera.frustum.pos - Reference).Normalized();
	X = math::Cross(math::float3(0.0f, 1.0f, 0.0f), Z).Normalized();
	Y = math::Cross(Z, X);

	camera.frustum.front = Z;
	camera.frustum.up = Y;

	if (distance != 0.0f)
	{
		float d2 = (camera.frustum.pos - Reference).Length();
		d2 -= distance;

		camera.frustum.Translate(camera.frustum.front * d2);
	}
}

void ModuleCamera3D::Orbit(math::float3 target, float deltaX, float deltaY)
{
	math::float3 X = math::float3(1.0f, 0.0f, 0.0f);
	math::float3 Y = math::float3(0.0f, 1.0f, 0.0f);
	math::float3 Z = math::float3(0.0f, 0.0f, 1.0f);

	Position -= target;

	if (deltaY != 0.0f)
	{
		math::float3x3 rotationMatrix = math::float3x3::RotateY(deltaY);
		X = rotationMatrix * X;
		Y = rotationMatrix * Y;
		Z = rotationMatrix * Z;
	}

	if (deltaX != 0.0f)
	{
		math::float3x3 rotationMatrix = math::float3x3::RotateAxisAngle(X, deltaX);
		Y = rotationMatrix * Y;
		Z = rotationMatrix * Z;
	}

	Position = target + (Z * Position.Length());
}

Camera::Camera()
{
	Init();
}

void Camera::Init()
{
	/* Set camera vars*/
	width = 16; // TODO, QUESTION, IS ASPECT RATIO GOOD NOW ??
	height = 9;
	aspect_ratio = width / height; // We set aspect ratio 16:9 by now

	near_plane = 0.2;
	far_plane = 1000;
	vertical_fov = 60; /* In degrees */
					   /* Set frustum vars */
	frustum.type = PerspectiveFrustum;
	frustum.pos.Set(0, 0, 0);
	frustum.front.Set(0, 0, 1);
	frustum.up.Set(0, 1, 0);
	frustum.nearPlaneDistance = near_plane;
	frustum.farPlaneDistance = far_plane;
	frustum.verticalFov = vertical_fov * DEGTORAD;
	frustum.horizontalFov = Atan(aspect_ratio*Tan(frustum.verticalFov / 2)) * 2;
	frustum_center = frustum.CenterPoint();
	frustum_halfdistance_squared = 170 * 170;
}

float* Camera::GetViewMatrix() const
{
	float4x4 matrix = frustum.ViewMatrix();
	return (float*)matrix.Transposed().v;
}

float* Camera::GetProjectionMatrix() const
{
	float4x4 matrix = frustum.ProjectionMatrix();
	return (float*)matrix.Transposed().v;
}

math::float4x4 Camera::GetOpenGlViewMatrix() const
{
	math::float4x4 matrix = frustum.ViewMatrix();
	return matrix.Transposed();
}

math::float4x4 Camera::GetOpenGlProjectionMatrix() const
{
	math::float4x4 matrix = frustum.ProjectionMatrix();
	return matrix.Transposed();
}