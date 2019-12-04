#ifndef __ModulePhysics_H__
#define __ModulePhysics_H__

#include <list>
#include "Module.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool			Start();
	update_status	PreUpdate(float dt);
	update_status	Update(float dt);
	bool			CleanUp();

	btRigidBody* CreateCollisionObject(math::float3 size);

private:
	btDiscreteDynamicsWorld*				physicsWorld			= nullptr;
	btDefaultCollisionConfiguration*		collisionConfiguration	= nullptr;
	btCollisionDispatcher*					collisionDispatcher		= nullptr;
	btBroadphaseInterface*					broadphaseInterface		= nullptr;
	btSequentialImpulseConstraintSolver*	constraintSolver		= nullptr;

};

#endif