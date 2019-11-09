#include "C_Camera.h"
#include "MathGeoLib-1.5\src\MathGeoLib.h"

C_Camera::C_Camera(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_CAMERA)
{
	isUnique = true;
	InitFrustum();
	debugCube = new DebugCube();
}

C_Camera::~C_Camera()
{

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
	if (renderCameraFrustum)
	{
		if (debugCube != nullptr)
		{
			static math::float3 frustumToDraw[8];
			frustum.GetCornerPoints(frustumToDraw);
			debugCube->DirectRender(frustumToDraw, White );
		}
	}
}

void C_Camera::Update(float dt)
{

}