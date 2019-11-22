#include "Application.h"
#include "ModuleResources.h"


// Resources Mesh
ResourceMesh::ResourceMesh(Mesh* mesh, uuid_unit uuid)
{
	this->mesh = mesh;
	this->uuid = uuid;
	this->totalInUse = 0;
}

ResourceMesh::~ResourceMesh()
{
	delete mesh;
}
// ~Resources Mesh

ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleResources";
}

ModuleResources::~ModuleResources()
{
	CleanUp();
}

bool ModuleResources::CleanUp()
{
	CleanResourceMeshes();
	return true;
}

void ModuleResources::AddResourceMesh(Mesh* mesh, uuid_unit uuid)
{
	if (mesh != nullptr)
	{
		if (GetResourceMesh(mesh->uuid) == nullptr)
		{
			ResourceMesh* resource = new ResourceMesh(mesh, uuid);
			resourcesMesh.push_back(resource);
		}
	}
}

void ModuleResources::DeleteResourceMesh(uuid_unit uuid)
{
	for (int i = 0; i < resourcesMesh.size(); i++)
	{
		if (resourcesMesh[i]->uuid == uuid)
		{
			if (resourcesMesh[i]->totalInUse <= 0)
			{
				resourcesMesh.erase(resourcesMesh.begin() + i);
			}
		}
	}
}

ResourceMesh* ModuleResources::GetResourceMesh(uuid_unit uuid)
{
	ResourceMesh* ret = nullptr;
	for (int i = 0; i < resourcesMesh.size(); i++)
	{
		if (resourcesMesh[i]->uuid == uuid)
		{
			ret = resourcesMesh[i];
		}
	}
	return ret;
}

void ModuleResources::CleanResourceMeshes()
{
	for (int i = 0; i < resourcesMesh.size(); i++)
	{
		resourcesMesh.erase(resourcesMesh.begin() + i);
	}
	resourcesMesh.clear();
}