#include "Application.h"
#include "ModuleImporter.h"
#include "Console.h"

#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/cimport.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Assimp/libx86/zlibstatic.lib")

void myCallback(const char* msg, char* userData)
{
	printf_s("%s", msg);
	// TODO: Passar aquest log a consola
	LOG("%s", msg);
}

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleImporter";
}

bool ModuleImporter::Start()
{
	struct aiLogStream stream;
	stream.callback = myCallback;
	aiAttachLogStream(&stream);

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
	json_object_set_string(root_object, "resources_directory", resources_directory.c_str());
	return true;
}

bool ModuleImporter::Load(JSON_Object* root_object)
{
	resources_directory = json_object_get_string(root_object, "resources_directory");
	return true;
}

void ModuleImporter::DistributeObjectToLoad(const char* path)
{
	std::string name_extension(path);
	name_extension = name_extension.substr(name_extension.find_last_of(92) + 1);
	std::string extension = name_extension;
	extension = extension.substr(extension.find_last_of('.') + 1);
	std::string finalPath = resources_directory + name_extension;

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

const std::string ModuleImporter::GetResourcesDirectory() const
{
	return resources_directory;
}

void ModuleImporter::SetResourcesDirectory(const std::string str)
{
	resources_directory = str;
}