#include "Application.h"
#include "ModuleFS.h"

ModuleFS::ModuleFS(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleFileSystem";
}

bool ModuleFS::Start()
{
	mesh_file_extension = ".smesh";

	sceneImporter = new SceneImporter();
	materialImporter = new MaterialImporter();

	if (!CheckExistingFolder(resources_directory.c_str()))
		CreateFolder(resources_directory.c_str());
	if (!CheckExistingFolder((resources_directory + "Assets").c_str()))
		CreateFolder((resources_directory + "Assets").c_str());
	if (!CheckExistingFolder((resources_directory + "Library").c_str()))
		CreateFolder((resources_directory + "Library").c_str());
	if (!CheckExistingFolder((resources_directory + "Library/" + "Meshes").c_str()))
		CreateFolder((resources_directory + "Library/" + "Meshes").c_str());
	if (!CheckExistingFolder((resources_directory + "Library/" + "Materials").c_str()))
		CreateFolder((resources_directory + "Library/" + "Materials").c_str());

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

bool ModuleFS::CheckExistingFolder(const char* path)
{
	DWORD dir = GetFileAttributesA(path);
	if (dir != INVALID_FILE_ATTRIBUTES)
		if (dir & FILE_ATTRIBUTE_DIRECTORY)
			return true;
	return false;
}

void ModuleFS::CreateFolder(const char* path)
{
	CreateDirectory(path, NULL);
}

unsigned int ModuleFS::CreateOwnMesh(Mesh* mesh)
{
	// Header
	unsigned int ranges[2] = { mesh->GetIndicesSize(), mesh->GetVerticesSize() };

	// Body
	unsigned int size = sizeof(ranges) + sizeof(unsigned int) * mesh->GetIndicesSize() * 3 + sizeof(GLfloat) * mesh->GetVerticesSize() * 3;

	// Data & Cursor
	char* data = new char[size]; // Contenidor
	char* cursor = data; // Apuntador

	// Add header
	unsigned int bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);

	// Put indices
	cursor += bytes;
	bytes = sizeof(unsigned int) * mesh->GetIndicesSize() * 3;
	memcpy(cursor, mesh->indicesArray, bytes);

	LOG_CONSOLE("BEFORE !!");
	for (int i = 0; i < mesh->GetIndicesSize() * 3; i++)
	{
		LOG_CONSOLE("%u", mesh->indicesArray[i]);
	}

	// =============================================
	//mesh->ClearIndicesArray();
	//mesh->indicesArray = new unsigned int[mesh->GetIndicesSize() * 3];
	//memcpy(mesh->indicesArray, cursor, bytes);
	//mesh->GenerateIndicesBuffer();

	//LOG_CONSOLE("AFTER 1.0 !!")
	//	for (int i = 0; i < mesh->GetIndicesSize(); i++)
	//	{
	//		LOG_CONSOLE("%u", mesh->indicesArray[i]);
	//	}
	// =============================================

	// Put vertices
	cursor += bytes;
	bytes = sizeof(GLfloat) * mesh->GetVerticesSize() * 3;
	memcpy(cursor, mesh->verticesArray, bytes);

	unsigned int nUUID = App->input->GenerateUUID();
	std::string newDirection = App->fs->resources_directory + "Library/" + "Meshes/" + std::to_string(nUUID) + mesh_file_extension;
	FILE* file = fopen(newDirection.c_str(), "w");
	fwrite(data, sizeof(char), size, file);
	fclose(file);

	cursor = nullptr;
	delete[] data;

	//sceneImporter->LoadMesh((App->fs->resources_directory + "Library/" + "Meshes/" + std::to_string(nUUID) + mesh_file_extension).c_str());

	return nUUID;
}

unsigned int ModuleFS::CreateOwnTexture(Texture* texture)
{
	unsigned int nUUID = App->input->GenerateUUID();

	return nUUID;
}