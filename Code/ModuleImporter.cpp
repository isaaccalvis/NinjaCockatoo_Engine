#include "Application.h"
#include "ModuleImporter.h"

#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Assimp/libx86/zlibstatic.lib")

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "importer";
}

bool ModuleImporter::Start()
{
	return true;
}

update_status ModuleImporter::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	return true;
}

bool ModuleImporter::Save(JSON_Object* root_object)
{
	return true;
}

bool ModuleImporter::Load(JSON_Object* root_object)
{
	return true;
}

void ModuleImporter::DistributeObjectToLoad(const char* path)
{
	std::string name_extension(path);
	name_extension = name_extension.substr(name_extension.find_last_of(92) + 1);
	std::string extension = name_extension;
	extension = extension.substr(extension.find_last_of('.') + 1);
	std::string finalPath = "Resources/" + name_extension;

	CopyFile(path, finalPath.c_str(), true);

	if (extension == "fbx")
	{
		LoadMesh(finalPath.c_str());
	}
}

void ModuleImporter::LoadMesh(const char* path)
{
	App->meshes->AddCustomMesh(path);
}