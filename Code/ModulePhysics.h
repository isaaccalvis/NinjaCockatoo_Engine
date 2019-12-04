#ifndef __ModulePhysics_H__
#define __ModulePhysics_H__

#include <list>
#include "Module.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

class PhysicsDebugDrawer;

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

	std::list<btRigidBody*> rigidbodies;

private:
	btDiscreteDynamicsWorld*				physicsWorld			= nullptr;
	btDefaultCollisionConfiguration*		collisionConfiguration	= nullptr;
	btCollisionDispatcher*					collisionDispatcher		= nullptr;
	btBroadphaseInterface*					broadphaseInterface		= nullptr;
	btSequentialImpulseConstraintSolver*	constraintSolver		= nullptr;

	PhysicsDebugDrawer*						physicsDebugDrawer = nullptr;
};

class PhysicsDebugDrawer : public btIDebugDraw
{
public:
	PhysicsDebugDrawer();
	
	void drawLine(const btVector3& p_origin, const btVector3& p_end, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	getDebugMode() const;

	DebugDrawModes debugMode = DebugDrawModes::DBG_DrawWireframe;
};

#endif