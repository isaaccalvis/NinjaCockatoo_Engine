#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "Primitive.h"

enum COLLISION_STATE {
	COLLISION_ENTER,
	COLLISION_EXIT
};

class btRigidBody;
class Module;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	PhysBody3D(btRigidBody* body, Sphere p, bool primitiveRender = false);
	PhysBody3D(btRigidBody* body, Cube c, bool primitiveRender = false);

	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetAsSensor(bool is_sensor);
	void Render();
private:
	btRigidBody* body = nullptr;
protected:
	bool is_sensor = false;
	COLLISION_STATE col_state = COLLISION_EXIT;
	int isCubeOrSpehere = 0; // 0 = nothing, 1 = sphere, 2 = cube
public:
	p2List<Module*> collision_listeners;
	Sphere*	pToRender = nullptr;
	Cube*	cToRender = nullptr;
	int renderThisPhysbodyBasicPrimitive = false;
};

#endif // __PhysBody3D_H__