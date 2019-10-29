#include "Application.h"
#include "ModuleFS.h"

ModuleFS::ModuleFS(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleFileSystem";
}

bool ModuleFS::Start()
{
	sceneImporter = new SceneImporter();
	materialImporter = new MaterialImporter();

	CheckAndGenerateResourcesFolders();
	return true;
}

bool ModuleFS::CleanUp()
{
	return true;
}

bool ModuleFS::Save(JSON_Object* root_object)
{
	json_object_set_string(root_object, "resources_directory", resources_directory.c_str());
	return true;
}

bool ModuleFS::Load(JSON_Object* root_object)
{
	resources_directory = json_object_get_string(root_object, "resources_directory");
	return true;
}

void ModuleFS::DistributeObjectToLoad(const char* path)
{
	std::string direction_without_name(path);
	direction_without_name = direction_without_name.substr(0, direction_without_name.find_last_of(92) + 1);
	std::string name_and_extension(path);
	name_and_extension = name_and_extension.substr(name_and_extension.find_last_of(92) + 1);
	std::string extension = name_and_extension;
	extension = extension.substr(extension.find_last_of('.') + 1);
	std::string toSavePath = resources_directory + name_and_extension;

	CopyFile(path, toSavePath.c_str(), true);

	if (extension == "fbx")
	{
		LoadScene(toSavePath.c_str(), direction_without_name.c_str());
	}
	else if (extension == "dds" || extension == "png")
	{
		materialImporter->Import(toSavePath.c_str());
	}
}

void ModuleFS::LoadScene(const char* path, const char* originalPath)
{
	SceneImporterSettings* settings = new SceneImporterSettings();
	settings->originalPath = originalPath;
	sceneImporter->Import(path, settings);
	delete settings;
}

void ModuleFS::CheckAndGenerateResourcesFolders()
{
	DWORD resDir = GetFileAttributesA(resources_directory.c_str());
	DWORD assDir = GetFileAttributesA((resources_directory + "Assets").c_str());
	DWORD libDir = GetFileAttributesA((resources_directory + "Library").c_str());
	DWORD libMeshDir = GetFileAttributesA((resources_directory + "Library/" + "Meshes").c_str());
	DWORD libMatDir = GetFileAttributesA((resources_directory + "Library/" + "Materials").c_str());

	if (resDir != INVALID_FILE_ATTRIBUTES)
	{
		if (assDir == INVALID_FILE_ATTRIBUTES)
		{
			CreateDirectory((resources_directory + "Assets").c_str(), NULL);
		}
		if (libDir != INVALID_FILE_ATTRIBUTES)
		{
			if (libMeshDir == INVALID_FILE_ATTRIBUTES)
			{
				CreateDirectory((resources_directory + "Library/" + "Meshes").c_str(), NULL);
			}
			if (libMatDir == INVALID_FILE_ATTRIBUTES)
			{
				CreateDirectory((resources_directory + "Library/" + "Material").c_str(), NULL);
			}
		}
		else
		{
			CreateDirectory((resources_directory + "Library").c_str(), NULL);
			CreateDirectory((resources_directory + "Library/" + "Meshes").c_str(), NULL);
			CreateDirectory((resources_directory + "Library/" + "Material").c_str(), NULL);
		}
	}
	else if (resDir == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectory((resources_directory).c_str(), NULL);
		CreateDirectory((resources_directory + "Assets").c_str(), NULL);
		CreateDirectory((resources_directory + "Library").c_str(), NULL);
		CreateDirectory((resources_directory + "Library/" + "Meshes").c_str(), NULL);
		CreateDirectory((resources_directory + "Library/" + "Material").c_str(), NULL);
	}
}