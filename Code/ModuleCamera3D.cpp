#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "C_Camera.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleCamera3D";
	camera = new Camera();
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
	if (isCameraEditor)
	{
		math::float3 newPos(0, 0, 0);

		if (App->input->GetMouseZ() != 0)
		{
			camera->frustum.Translate(App->input->GetMouseZ() * camera->frustum.front * mouse_wheel_speed);
		}

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			if (App->scene->goSelected != nullptr)
			{
				App->scene->MakeCameraLookThisGOSelected();
			}
		}

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			float speed = camera_mov_speed * dt;
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				speed *= 2;
			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += camera->frustum.up * speed;
			if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos -= camera->frustum.up *speed;
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += camera->frustum.front * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= camera->frustum.front * speed;
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= camera->frustum.WorldRight() * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += camera->frustum.WorldRight() * speed;
		}

		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			if (App->scene->goSelected != nullptr)
			{
				int dx = -App->input->GetMouseXMotion();
				int dy = -App->input->GetMouseYMotion();

				float deltaX = (float)dx * mouse_sensitivity * dt;
				float deltaY = (float)dy * mouse_sensitivity * dt;
				Orbit(App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position, deltaY, deltaX);
			}
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			if (dx != 0 || dy != 0)
			{
				math::Quat rotationX = math::Quat::RotateAxisAngle({ 0.0f,1.0f,0.0f }, dx * DEGTORAD * mouse_sensitivity);
				math::Quat rotationY = math::Quat::RotateAxisAngle(camera->frustum.WorldRight(), dy * DEGTORAD * mouse_sensitivity);
				math::Quat finalRotation = rotationX * rotationY;

				camera->frustum.up = finalRotation * camera->frustum.up;
				camera->frustum.front = finalRotation * camera->frustum.front;

				float distance = (camera->frustum.pos - camera->frustum.pos).Length();
				camera->frustum.pos = camera->frustum.pos + (-camera->frustum.front * distance);
			}
		}
		camera->frustum.Translate(newPos);
	}

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::LookAt( const math::float3 &Spot, float distance)
{
	math::float3 X = math::float3(1.0f, 0.0f, 0.0f);
	math::float3 Y = math::float3(0.0f, 1.0f, 0.0f);
	math::float3 Z = math::float3(0.0f, 0.0f, 1.0f);

	Z = -(camera->frustum.pos - Spot).Normalized();
	X = math::Cross(math::float3(0.0f, 1.0f, 0.0f), Z).Normalized();
	Y = math::Cross(Z, X);

	camera->frustum.front = Z;
	camera->frustum.up = Y;

	if (distance != 0.0f)
	{
		float d2 = (camera->frustum.pos - Spot).Length();
		d2 -= distance;

		camera->frustum.Translate(camera->frustum.front * d2);
	}
}

void ModuleCamera3D::Orbit(math::float3 target, float deltaX, float deltaY)
{
	math::Quat rotationX = math::Quat::RotateAxisAngle({ 0.0f,1.0f,0.0f }, deltaY * mouse_sensitivity);
	math::Quat rotationY = math::Quat::RotateAxisAngle(camera->frustum.WorldRight(), deltaX * mouse_sensitivity);
	math::Quat finalRotation = rotationX * rotationY;

	camera->frustum.up = finalRotation * camera->frustum.up;
	camera->frustum.front = finalRotation * camera->frustum.front;

	float distance = (camera->frustum.pos - target).Length();
	camera->frustum.pos = target + (-camera->frustum.front * distance);
}

Camera::Camera()
{
	Init();
}

void Camera::Init()
{
	width = 16;
	height = 9;
	aspect_ratio = width / height;

	near_plane = 0.2;
	far_plane = 1000;
	vertical_fov = 60;
	
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

math::float4x4 Camera::GetViewMatrix() const
{
	math::float4x4 matrix = frustum.ViewMatrix();
	return matrix.Transposed();
}

math::float4x4 Camera::GetProjectionMatrix() const
{
	math::float4x4 matrix = frustum.ProjectionMatrix();
	return matrix.Transposed();
}