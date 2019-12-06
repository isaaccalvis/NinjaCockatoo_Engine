#include "Application.h"
#include "ModulePhysics.h"

#define PHYSICS_DEBUG

#ifdef PHYSICS_DEBUG
#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#else
#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")
#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
#pragma comment (lib, "Bullet/libx86/LinearMath.lib")
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModulePhysics";

	collisionConfiguration = new btDefaultCollisionConfiguration();
	collisionDispatcher = new btCollisionDispatcher(collisionConfiguration);
	broadphaseInterface = new btDbvtBroadphase();
	constraintSolver = new btSequentialImpulseConstraintSolver();
	physicsDebugDrawer = new PhysicsDebugDrawer();
}

ModulePhysics::~ModulePhysics()
{
	delete constraintSolver;
	delete broadphaseInterface;
	delete collisionDispatcher;
	delete collisionConfiguration;
}

bool ModulePhysics::Start()
{
	LOG_CONSOLE("Creating Physics World");
	physicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher,
		broadphaseInterface, constraintSolver, collisionConfiguration);
	physicsWorld->setGravity(gravity);
	physicsWorld->setDebugDrawer(physicsDebugDrawer);

	return true;
}

update_status ModulePhysics::PreUpdate(float dt)
{
	if (activePhysics)
	{
		physicsWorld->stepSimulation(dt);
		int numManifolds = physicsWorld->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = physicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		}
	}
	return update_status::UPDATE_CONTINUE;
}

update_status ModulePhysics::Update(float dt)
{
	if (physicsDebugDraw)
	{
		physicsWorld->debugDrawWorld();
	}
	return update_status::UPDATE_CONTINUE;
}

bool ModulePhysics::CleanUp()
{
	// TODO: CLEAR LISTS
	return true;
}

btRigidBody* ModulePhysics::CreateRigidBody(PHYSIC_PRIMITIVE primitive, math::float3 size, float mass)
{
	// Shape
	btCollisionShape* shape;

	switch (primitive)
	{
	case PHYSIC_PRIMITIVE::PHY_NONE:
	{
		shape = new btEmptyShape();
	}
	break;
	case PHYSIC_PRIMITIVE::PHY_CUBE:
	{
		shape = new btBoxShape(btVector3(size.x / 2, size.y / 2, size.z / 2));
	}
	break;
	case PHYSIC_PRIMITIVE::PHY_SPHERE:
	{
		shape = new btSphereShape(size.x);
	}
	break;
	}

	// RigidBody
	btDefaultMotionState* myMotionState = new btDefaultMotionState();
	btRigidBody* rigidBody = new btRigidBody(mass, myMotionState, shape);

	physicsWorld->addRigidBody(rigidBody);
	rigidBodies.push_back(rigidBody);
	return rigidBody;
}

btTypedConstraint* ModulePhysics::CreateConstraint(btRigidBody* bodyA, btRigidBody* bodyB, const math::float3 pivotA, const math::float3 pivotB)
{
	btTypedConstraint* constraint = new btPoint2PointConstraint(
	*bodyA, *bodyB, btVector3(pivotA.x, pivotA.y, pivotA.z), btVector3(pivotB.x, pivotB.y, pivotB.z));
	constraints.push_back(constraint);
	physicsWorld->addConstraint(constraint);
	return constraint;
}

void ModulePhysics::DeleteRigidBody(btRigidBody* rigidBody)
{
	physicsWorld->removeRigidBody(rigidBody);
	rigidBodies.remove(rigidBody);
}

// ================== DEBUG DRAW ==================

PhysicsDebugDrawer::PhysicsDebugDrawer()
{

}

void PhysicsDebugDrawer::drawLine(const btVector3& p_origin, const btVector3& p_end, const btVector3& color)
{
	glPushMatrix();

	glColor3f(color.getX(), color.getY(), color.getZ());
	glLineWidth(3);

	glBegin(GL_LINES);

	glVertex3fv(p_origin);
	glVertex3fv(p_end);

	glEnd();
	
	glLineWidth(1);
	glPopMatrix();
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	glPushMatrix();

	glColor3f(color.getX() , color.getY(), color.getZ());
	glPointSize(3);

	glBegin(GL_POINT);

	glVertex3fv(PointOnB);

	glEnd();

	glPointSize(1);
	glPopMatrix();
}

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG_CONSOLE("Bullet Warning: %s", warningString);
}

void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG_CONSOLE("Bullet Log: %s", textString);
}

void PhysicsDebugDrawer::setDebugMode(int debugMode)
{
	this->debugMode = (DebugDrawModes)debugMode;
}

int	PhysicsDebugDrawer::getDebugMode() const
{
	return debugMode;
}