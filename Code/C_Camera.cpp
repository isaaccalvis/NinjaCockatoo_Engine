#include "C_Camera.h"
#include "MathGeoLib-1.5\src\MathGeoLib.h"

C_Camera::C_Camera(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_MATERIAL)
{
	isUnique = true;
	InitFrustum();
}

C_Camera::~C_Camera()
{

}

void C_Camera::InitFrustum()
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

void C_Camera::Update(float dt)
{

}