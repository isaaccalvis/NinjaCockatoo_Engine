#include "Application.h"
#include "ModuleMeshes.h"

#include "glew-2.1.0/include/GL/glew.h"

#include "MeshCube.h"
#include "MeshPlane.h"
#include "MeshSphere.h"

ModuleMeshes::ModuleMeshes(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleMeshes";
}

ModuleMeshes::~ModuleMeshes()
{}

bool ModuleMeshes::Start()
{
	return true;
}

update_status ModuleMeshes::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleMeshes::CleanUp()
{
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
	meshes.push_back(mesh);
}

void ModuleMeshes::AddCube()
{
	Mesh *tmp_mesh = new MeshCube();
	AddMesh(tmp_mesh);
}

void ModuleMeshes::AddPlane()
{
	Mesh *tmp_mesh = new MeshPlane();
	AddMesh(tmp_mesh);
}

void ModuleMeshes::AddSphere()
{
	Mesh *tmp_mesh = new MeshSphere(12, 24);
	AddMesh(tmp_mesh);
}