#include "Application.h"
#include "C_Camera.h"
#include "C_Transform.h"
#include "MathGeoLib-1.5\src\MathGeoLib.h"

C_Camera::C_Camera(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_CAMERA)
{
	isUnique = true;
	InitFrustum();
	debugCube = new DebugCube();
	if (App->scene->camera == nullptr)
	{
		App->scene->camera = parent;
		isMainCamera = true;
	}
	else
	{
		isMainCamera = false;
	}
}

C_Camera::~C_Camera()
{

}

void C_Camera::Update(float dt)
{
	UpdateTransform();
	if (renderCameraFrustum)
	{
		RenderCamera();
	}
}

void C_Camera::OnSaveJson(JSON_Object* object)
{
	if (parent != nullptr)
	{
		json_object_set_string(object, "c_type", "c_camera");
		json_object_set_number(object, "near_plane_distance", frustum.nearPlaneDistance);
		json_object_set_number(object, "far_plane_distance", frustum.farPlaneDistance);
		json_object_set_number(object, "vertical_foc", frustum.verticalFov);
		json_object_set_boolean(object, "is_main_camera", isMainCamera);
		json_object_set_boolean(object, "frustum_culling", frustumCulling);
	}
}

void C_Camera::OnLoadJson(JSON_Object* object)
{
	frustum.nearPlaneDistance = json_object_get_number(object, "near_plane_distance");
	frustum.farPlaneDistance = json_object_get_number(object, "far_plane_distance");
	frustum.verticalFov = json_object_get_number(object, "vertical_foc");
	isMainCamera = json_object_get_boolean(object, "is_main_camera");
	if (isMainCamera)
		App->scene->camera = this->parent;
	frustumCulling = json_object_get_boolean(object, "frustum_culling");
}

void C_Camera::InitFrustum()
{
	width = 16;
	height = 9;
	aspect_ratio = width / height;

	near_plane = 0.2;
	far_plane = 1000;
	vertical_fov = 60; /* In degrees */

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

void C_Camera::RenderCamera()
{
	if (debugCube != nullptr)
	{
		static math::float3 frustumToDraw[8];
		frustum.GetCornerPoints(frustumToDraw);
		debugCube->DirectRender(frustumToDraw, White);
	}
}

void C_Camera::UpdateTransform()
{
	math::float4x4 matrix = parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->GetGlobalMatrix();
	frustum.pos = matrix.TranslatePart();
	frustum.front = matrix.WorldZ();
	frustum.up = matrix.WorldY();
}

void C_Camera::BecomeMainCamera()
{
	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		if (App->scene->gameObjects[i]->GetComponent(COMPONENT_CAMERA) != nullptr)
		{
			App->scene->gameObjects[i]->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->isMainCamera = false;
		}
	}
	App->scene->camera = parent;
	isMainCamera = true;
}

bool C_Camera::IsInsideFrustumCulling(GameObject* go)
{
	//for (int i = 0; i < 8; ++i)
	//{
		// This is faster, but have some problems
		//if (frustum.Contains(go->boundingBox.CornerPoint(i)))
	if (frustum.Intersects(go->boundingBox))
	{
		return true;
	}
	//}
	return false;
}