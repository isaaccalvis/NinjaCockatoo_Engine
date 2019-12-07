#ifndef __ModulePhysics_H__
#define __ModulePhysics_H__

#include <list>
#include "Module.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "MathGeoLib-1.5/src/Math/float3.h"

enum PHYSIC_PRIMITIVE
{
	PHY_NONE = 0,
	PHY_CUBE,
	PHY_SPHERE
};

enum PHYSIC_CONSTRAINT
{
	CONSTRAINT_P2P,
	CONSTRAINT_HINGE,
	CONSTRAINT_SLIDER
};

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

	btRigidBody* CreateRigidBody(PHYSIC_PRIMITIVE primitive, math::float3 size, float mass = 0.0f);
	btTypedConstraint* CreateConstraint(PHYSIC_CONSTRAINT type,
		btRigidBody* bodyA, btRigidBody* bodyB,
		const math::float3 pivotA, const math::float3 pivotB,
		const math::float3 axisA = math::float3::zero,
		const math::float3 axisB = math::float3::zero);

	void DeleteShape(btCollisionShape* shape);
	void DeleteRigidBody(btRigidBody* rigidBody);
	void DeleteConstraint(btTypedConstraint* constraint);

	void ClearShapes();
	void ClearRigidBodies();
	void ClearConstraints();

public:
	std::list<btCollisionShape*> shapes;
	std::list<btRigidBody*> rigidBodies;
	std::list<btTypedConstraint*> constraints;

	btVector3 gravity = btVector3(0, -9.8f, 0);

	bool activePhysics = false;
	bool physicsDebugDraw = true;

	// TODO: PASSARHO A PRIVAT
public:
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