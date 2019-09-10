#include "Application.h"
#include "ModuleMeshes.h"

#include "glew-2.1.0/include/GL/glew.h"


#include "MeshCube.h"


ModuleMeshes::ModuleMeshes(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleMeshes";
}

ModuleMeshes::~ModuleMeshes()
{}

bool ModuleMeshes::Start()
{
	Mesh *tmp_cube_mesh = new MeshCube(3);
	AddMesh(tmp_cube_mesh);
	return true;
}

update_status ModuleMeshes::Update(float dt)
{
	// Render phase
	std::list<Mesh*>::iterator it = meshes.begin();
	for (; it != meshes.end(); it++)
	{
		(*it)->Render();
	}

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