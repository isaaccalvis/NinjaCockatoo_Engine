#include "Application.h"
#include "ModulePhysics.h"

#include "C_Collider.h"
#include "C_RigidBody.h"

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
	// Clean colliding go list
	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		if (App->scene->gameObjects[i]->GetComponent(COMPONENT_RIGIDBODY) != nullptr)
		{
			App->scene->gameObjects[i]->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->ClearCollidingGameObjectList();
		}
		else if (App->scene->gameObjects[i]->GetComponent(COMPONENT_COLLIDER) != nullptr)
		{
			App->scene->gameObjects[i]->GetComponent(COMPONENT_COLLIDER)->GetComponentAsCollider()->ClearCollidingGameObjectList();
		}
	}

	if (activePhysics)
	{
		physicsWorld->stepSimulation(dt);
		int numManifolds = physicsWorld->getDispatcher()->getNumManifolds();
		LOG_CONSOLE("Collisions: %i", numManifolds);
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = physicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* colA = (btCollisionObject*)contactManifold->getBody0();
			btCollisionObject* colB = (btCollisionObject*)contactManifold->getBody1();

			// Check from who is that collision
			GameObject* objA = nullptr;
			GameObject* objB = nullptr;
			for (int a = 0; a < App->scene->gameObjects.size(); a++)
			{
				if (App->scene->gameObjects[a]->GetComponent(COMPONENT_COLLIDER) != nullptr)
				{
					if (App->scene->gameObjects[a]->GetComponent(COMPONENT_COLLIDER)->GetComponentAsCollider()->rigidBody == colA)
					{
						objA = App->scene->gameObjects[a];
					}
					else if (App->scene->gameObjects[a]->GetComponent(COMPONENT_COLLIDER)->GetComponentAsCollider()->rigidBody == colB)
					{
						objB = App->scene->gameObjects[a];
					}
				}
				else if (App->scene->gameObjects[a]->GetComponent(COMPONENT_RIGIDBODY) != nullptr)
				{
					if (App->scene->gameObjects[a]->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody == colA)
					{
						objA = App->scene->gameObjects[a];
					}
					else if (App->scene->gameObjects[a]->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody == colB)
					{
						objB = App->scene->gameObjects[a];
					}
				}
			}

			if (objA != nullptr && objB != nullptr)
			{
				if (objA->GetComponent(COMPONENT_COLLIDER) != nullptr)
				{
					objA->GetComponent(COMPONENT_COLLIDER)->GetComponentAsCollider()->AddCollidingGameObjectList(objB);
				}
				else
				{
					objA->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->AddCollidingGameObjectList(objB);
				}
				if (objB->GetComponent(COMPONENT_COLLIDER) != nullptr)
				{
					objB->GetComponent(COMPONENT_COLLIDER)->GetComponentAsCollider()->AddCollidingGameObjectList(objA);
				}
				else
				{
					objB->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->AddCollidingGameObjectList(objA);
				}
			}

			//unsigned int numContacts = contactManifold->getNumContacts();
			//if (numContacts > 0)
			//{

			//}
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
	ClearConstraints();
	ClearRigidBodies();
	ClearShapes();
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
		shape = new btSphereShape(size.x / 2);
	}
	break;
	}
	shapes.push_back(shape);

	// RigidBody
	btDefaultMotionState* myMotionState = new btDefaultMotionState();
	btRigidBody* rigidBody = new btRigidBody(mass, myMotionState, shape);

	physicsWorld->addRigidBody(rigidBody);
	rigidBodies.push_back(rigidBody);
	return rigidBody;
}

btTypedConstraint* ModulePhysics::CreateConstraint(
	PHYSIC_CONSTRAINT type,
	btRigidBody* bodyA, btRigidBody* bodyB, 
	const math::float3 pivotA, const math::float3 pivotB,
	const math::float3 axisA, const math::float3 axisB)
{
	btTypedConstraint* constraint;
	switch (type)
	{
	case PHYSIC_CONSTRAINT::CONSTRAINT_P2P:
	{
		constraint = new btPoint2PointConstraint(
			*bodyA, *bodyB, btVector3(pivotA.x, pivotA.y, pivotA.z), btVector3(pivotB.x, pivotB.y, pivotB.z));
	}
	break;
	case PHYSIC_CONSTRAINT::CONSTRAINT_HINGE:
	{
		constraint = new btHingeConstraint(
			*bodyA, *bodyB,
			btVector3(pivotA.x, pivotA.y, pivotA.z),
			btVector3(pivotB.x, pivotB.y, pivotB.z),
			btVector3(axisA.x, axisA.y, axisA.z),
			btVector3(axisB.x, axisB.y, axisB.z));

	}
	break;
	case PHYSIC_CONSTRAINT::CONSTRAINT_SLIDER:
	{
		btTransform transform1;
		btTransform transform2;
		transform1.setOrigin(btVector3(pivotA.x, pivotA.y, pivotA.z));
		transform2.setOrigin(btVector3(pivotB.x, pivotB.y, pivotB.z));
		constraint = new btSliderConstraint(
			*bodyA, *bodyB,
			transform1,
			transform2, false);
	}
	break;
	}
	constraint->setDbgDrawSize(1.0f);
	constraints.push_back(constraint);
	physicsWorld->addConstraint(constraint);
	return constraint;
}

void ModulePhysics::DeleteShape(btCollisionShape* shape)
{
	if (shape != nullptr)
	{
		shapes.remove(shape);
	}
}

void ModulePhysics::DeleteRigidBody(btRigidBody* rigidBody)
{
	if (rigidBody != nullptr)
	{
		physicsWorld->removeRigidBody(rigidBody);
		rigidBodies.remove(rigidBody);
	}
}

void ModulePhysics::DeleteConstraint(btTypedConstraint* constraint)
{
	if (constraint != nullptr)
	{
		physicsWorld->removeConstraint(constraint);
		constraints.remove(constraint);
	}
}

void ModulePhysics::ClearShapes()
{
	for (std::list<btCollisionShape*>::iterator it = shapes.begin();
		it != shapes.end(); it++)
	{
		DeleteShape((*it));
	}
}

void ModulePhysics::ClearRigidBodies()
{
	for (std::list<btRigidBody*>::iterator it = rigidBodies.begin();
		it != rigidBodies.end(); it++)
	{
		DeleteRigidBody((*it));
	}
}

void ModulePhysics::ClearConstraints()
{
	for (std::list<btTypedConstraint*>::iterator it = constraints.begin();
		it != constraints.end(); it++)
	{
		DeleteConstraint((*it));
	}
}


btVector3 ModulePhysics::GetGravity() const
{
	return gravity;
}

void ModulePhysics::SetGravity(btVector3 nG)
{
	gravity = nG;
	physicsWorld->setGravity(gravity);
	for (std::list<btRigidBody*>::iterator it = rigidBodies.begin();
		it != rigidBodies.end(); it++)
	{
		(*it)->setGravity(gravity);
	}
}

void ModulePhysics::ClearForces()
{
	for (std::list<btRigidBody*>::iterator it = rigidBodies.begin();
		it != rigidBodies.end(); it++)
	{
		(*it)->setLinearVelocity(btVector3(0, 0, 0));
		(*it)->setAngularVelocity(btVector3(0, 0, 0));
		(*it)->clearForces();
	}
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