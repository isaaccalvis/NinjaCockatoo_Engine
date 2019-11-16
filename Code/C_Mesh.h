#ifndef __C_Mesh_H__
#define __C_Mesh_H__

#include "Globals.h"
#include "Component.h"
#include "Mesh.h"

class C_Mesh : public Component
{
public:
	C_Mesh(GameObject* parent);
	~C_Mesh();

	void Update(float dt);

	void OnSaveJson(JSON_Object* object);

	void SetMesh(Mesh* mesh);
	Mesh* GetMesh();

private:
	Mesh* mesh = nullptr;
	uuid_unit mesh_resources_uuid = 0;
};

#endif