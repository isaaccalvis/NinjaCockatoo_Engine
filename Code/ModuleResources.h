#ifndef __ModuleResources_H__
#define __ModuleResources_H__

#include <vector>
#include "Module.h"
#include "Mesh.h"

class ResourceMesh
{
public:
	ResourceMesh(Mesh* mesh = nullptr, uuid_unit uuid = 0);
	~ResourceMesh();

public:
	Mesh* mesh = nullptr;
	uuid_unit uuid = 0;
	unsigned int totalInUse = 0;
};

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	bool CleanUp();

	// Meshes
	void AddResourceMesh(Mesh* mesh, uuid_unit uuid);
	void DeleteResourceMesh(uuid_unit uuid);
	ResourceMesh* GetResourceMesh(uuid_unit uuid);
	void CleanResourceMeshes();

private:
	std::vector<ResourceMesh*> resourcesMesh;
};

#endif