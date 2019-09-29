#include "Application.h"
#include "ModuleMeshes.h"

#include "glew-2.1.0/include/GL/glew.h"

#include "MeshCube.h"
#include "MeshPlane.h"
#include "MeshSphere.h"
#include "MeshCustom.h"
#include "MeshFrustrum.h"
#include "MeshCylinder.h"
#include "MeshDebugCube.h"

#include "mmgr/mmgr.h"

ModuleMeshes::ModuleMeshes(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleMeshes";
}

ModuleMeshes::~ModuleMeshes()
{

}

update_status ModuleMeshes::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_STATE::KEY_DOWN)
	{
		CleanUp();
	}
	return update_status::UPDATE_CONTINUE;
}

bool ModuleMeshes::CleanUp()
{
	lastMesh = nullptr;
	for (std::list<Mesh*>::iterator item = meshes.begin(); item != meshes.end(); item++)
	{
		delete (*item);
	}
	meshes.clear();
	return true;
}

bool ModuleMeshes::Save(JSON_Object* root_object)
{
	return true;
}

bool ModuleMeshes::Load(JSON_Object* root_object)
{
	return true;
}

void ModuleMeshes::AddMesh(Mesh* mesh)
{
	if (mesh != nullptr)
	{
		meshes.push_back(mesh);
		// TODO: TREURE LAST MESH
		lastMesh = mesh;
	}
}

void ModuleMeshes::DeleteMesh(Mesh* mesh)
{
	for (std::list<Mesh*>::iterator item = meshes.end(); item != meshes.begin();)
	{
		if ((*item) == mesh)
		{
			item--;
			meshes.erase(item++);
		}
	}
}

Mesh* ModuleMeshes::AddCube()
{
	Mesh *tmp_mesh = new MeshCube();
	AddMesh(tmp_mesh);
	return tmp_mesh;
}

Mesh* ModuleMeshes::AddFrustrum()
{
	Mesh *tmp_mesh = new MeshFrustrum();
	AddMesh(tmp_mesh);
	return tmp_mesh;
}

Mesh* ModuleMeshes::AddCylinder()
{
	Mesh *tmp_mesh = new MeshCylinder();
	AddMesh(tmp_mesh);
	return tmp_mesh;
}


Mesh* ModuleMeshes::AddDebugCube()
{
	Mesh *tmp_mesh = new MeshDebugCube();
	AddMesh(tmp_mesh);
	return tmp_mesh;
}

Mesh* ModuleMeshes::AddPlane()
{
	Mesh *tmp_mesh = new MeshPlane();
	AddMesh(tmp_mesh);
	return tmp_mesh;
}

Mesh* ModuleMeshes::AddSphere()
{
	Mesh *tmp_mesh = new MeshSphere(12, 24);
	AddMesh(tmp_mesh);
	return tmp_mesh;
}