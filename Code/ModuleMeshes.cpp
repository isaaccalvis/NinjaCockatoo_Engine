#include "Application.h"
#include "ModuleMeshes.h"

#include "glew-2.1.0/include/GL/glew.h"

#include "MeshCube.h"
#include "MeshPlane.h"
#include "MeshSphere.h"
#include "MeshCustom.h"
#include "MeshFrustrum.h"

#include "MeshDebugCube.h"

ModuleMeshes::ModuleMeshes(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleMeshes";
}

ModuleMeshes::~ModuleMeshes()
{

}

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
	if (mesh != nullptr)
	{
		meshes.push_back(mesh);
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

// TODO: hauriem de revisar que no fos nullptr ?
Mesh* ModuleMeshes::AddCustomMesh(const char* path)
{
	MeshCustom* tmp_mesh = new MeshCustom(path);
	AddMesh(tmp_mesh);
	return tmp_mesh;
}