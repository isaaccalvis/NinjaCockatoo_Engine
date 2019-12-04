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
	physicsWorld->setGravity(btVector3(0,0,-1));

	CreateCollisionObject(math::float3(2, 2, 2));
	CreateCollisionObject(math::float3(1, 1, 1));

	return true;
}

update_status ModulePhysics::PreUpdate(float dt)
{
	physicsWorld->stepSimulation(dt);
	int numManifolds = physicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = physicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		LOG_CONSOLE("%i", contactManifold->getNumContacts());
	}
	return update_status::UPDATE_CONTINUE;
}

update_status ModulePhysics::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModulePhysics::CleanUp()
{
	return true;
}

btRigidBody* ModulePhysics::CreateCollisionObject(math::float3 size)
{
	// Shape
	btCollisionShape* shape = new btBoxShape(btVector3(size.x, size.y, size.z));

	// RigidBody
	btDefaultMotionState* myMotionState = new btDefaultMotionState();
	btRigidBody* rigidBody = new btRigidBody(0 , myMotionState, shape);

	physicsWorld->addRigidBody(rigidBody);
	return rigidBody;
}