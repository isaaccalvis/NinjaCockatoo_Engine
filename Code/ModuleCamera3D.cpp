#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleCamera3D";

	CalculateViewMatrix();

	X = math::float3(1.0f, 0.0f, 0.0f);
	Y = math::float3(0.0f, 1.0f, 0.0f);
	Z = math::float3(0.0f, 0.0f, 1.0f);

	Position = math::float3(0.0f, 0.0f, 5.0f);
	Reference = math::float3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Init(JSON_Object* root_object)
{
	return true;
}

bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

update_status ModuleCamera3D::Update(float dt)
{
	math::float3 newPos(0,0,0);
	float speed = 3.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	Position += newPos;
	Reference += newPos;

	// Mouse motion

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = math::float3x3::RotateY(DeltaX) * X;
			Y = math::float3x3::RotateY(DeltaX) * Y;
			Z = math::float3x3::RotateY(DeltaX) * Z;
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = math::float3x3::RotateAxisAngle(X, DeltaY) * Y;
			Z = math::float3x3::RotateAxisAngle(X, DeltaY) * Z;

			if(Y.y < 0.0f)
			{
				Z = math::float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = math::Cross(Z, X);
			}
		}

		Position = Reference + Z * Position.Length();
	}

	// Recalculate matrix
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::Look(const math::float3 &Position, const math::float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = math::Cross(math::float3(0.0f, 1.0f, 0.0f), Z).Normalized();
	Y = math::Cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

void ModuleCamera3D::LookAt( const math::float3 &Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = math::Cross(math::float3(0.0f, 1.0f, 0.0f), Z).Normalized();
	Y = math::Cross(Z, X);

	CalculateViewMatrix();
}

void ModuleCamera3D::Move(const math::float3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

float* ModuleCamera3D::GetViewMatrix()
{
	return ViewMatrix.ptr();
}

void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = math::float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -math::Dot(X, Position), -math::Dot(Y, Position), -math::Dot(Z, Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}

Camera::Camera()
{
	Init();
}

void Camera::Init()
{
	/* Set camera vars*/
	width = 16;
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
	static float4x4 matrix;
	matrix = frustum.ViewMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

float* Camera::GetProjectionMatrix() const
{
	static float4x4 matrix;
	matrix = frustum.ProjectionMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}