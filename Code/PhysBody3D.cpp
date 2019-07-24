#include "PhysBody3D.h"
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{
	body->setUserPointer(this);
}
PhysBody3D::PhysBody3D(btRigidBody* body, Sphere p, bool primitiveRender) : body(body)
{
	body->setUserPointer(this);
	if (primitiveRender == true) {
		isCubeOrSpehere = 1;
		pToRender = new Sphere(p);
	}
}
PhysBody3D::PhysBody3D(btRigidBody* body, Cube c, bool primitiveRender) : body(body)
{
	body->setUserPointer(this);
	if (primitiveRender == true) {
		isCubeOrSpehere = 2;
		cToRender = new Cube(c);
	}
}
// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	if (pToRender != nullptr)
		delete pToRender;
	if (cToRender != nullptr)
		delete cToRender;
	delete body;
}

// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}

void PhysBody3D::SetAsSensor(bool is_sensor)
{
	if (this->is_sensor != is_sensor)
	{
		this->is_sensor = is_sensor;
		if (is_sensor == true)
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			body->setCollisionFlags(body->getCollisionFlags() &~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

void PhysBody3D::Render() {
	switch (isCubeOrSpehere) {
	case 1:
		pToRender->SetPos(body->getWorldTransform().getOrigin().getX(), body->getWorldTransform().getOrigin().getY(), body->getWorldTransform().getOrigin().getZ());
		pToRender->Render();
		break;
	case 2:
		cToRender->SetPos(body->getWorldTransform().getOrigin().getX(), body->getWorldTransform().getOrigin().getY(), body->getWorldTransform().getOrigin().getZ());
		cToRender->Render();
		break;
	}
}