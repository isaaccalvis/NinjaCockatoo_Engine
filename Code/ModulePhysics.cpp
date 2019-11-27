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
	return true;
}

update_status ModulePhysics::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModulePhysics::CleanUp()
{
	return true;
}