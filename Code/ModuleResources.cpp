#include "Application.h"
#include "ModuleResources.h"


// Resource Mesh
ResourceMesh::ResourceMesh(Mesh* mesh, uuid_unit uuid)
{
	this->mesh = mesh;
	this->uuid = uuid;
	mesh->uuid = uuid;
	this->totalInUse = 0u;
}

ResourceMesh::~ResourceMesh()
{
	delete mesh;
}

// Resource Material
ResourceMaterial::ResourceMaterial(Texture* texture, uuid_unit uuid)
{
	this->texture = texture;
	this->uuid = uuid;
	this->totalInUse = 0u;
}

ResourceMaterial::~ResourceMaterial()
{
	delete texture;
}

// Module Resources
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
	CleanResourceMaterial();
	return true;
}

void ModuleResources::AddResourceMesh(Mesh* mesh, uuid_unit uuid)
{
	if (mesh != nullptr)
	{
		if (GetResourceMesh(uuid) == nullptr)
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

ResourceMesh* ModuleResources::GetResourceMesh(std::string name)
{
	ResourceMesh* ret = nullptr;
	for (int i = 0; i < resourcesMesh.size(); i++)
	{
		if (resourcesMesh[i]->name.compare(name) == 0)
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

void ModuleResources::AddResourceMaterial(Texture* texture, uuid_unit uuid)
{
	if (texture != nullptr)
	{
		if (GetResourceMaterial(uuid) == nullptr)
		{
			ResourceMaterial* resource = new ResourceMaterial(texture, uuid);
			resourceMaterial.push_back(resource);
		}
	}
}

void ModuleResources::DeleteResourceMaterial(uuid_unit uuid)
{
	for (int i = 0; i < resourceMaterial.size(); i++)
	{
		if (resourceMaterial[i]->uuid == uuid)
		{
			if (resourceMaterial[i]->totalInUse <= 0)
			{
				resourceMaterial.erase(resourceMaterial.begin() + i);
			}
		}
	}
}

ResourceMaterial* ModuleResources::GetResourceMaterial(uuid_unit uuid)
{
	ResourceMaterial* ret = nullptr;
	for (int i = 0; i < resourceMaterial.size(); i++)
	{
		if (resourceMaterial[i]->uuid == uuid)
		{
			ret = resourceMaterial[i];
		}
	}
	return ret;
}

ResourceMaterial* ModuleResources::GetResourceMaterial(std::string name)
{
	ResourceMaterial* ret = nullptr;
	for (int i = 0; i < resourceMaterial.size(); i++)
	{
		if (resourceMaterial[i]->name.compare(name) == 0)
		{
			ret = resourceMaterial[i];
		}
	}
	return ret;
}

void ModuleResources::CleanResourceMaterial()
{
	for (int i = 0; i < resourceMaterial.size(); i++)
	{
		resourceMaterial.erase(resourceMaterial.begin() + i);
	}
	resourceMaterial.clear();
}