#include "Application.h"
#include "ModuleFS.h"

#include "physfs/include/physfs.h"

ModuleFS::ModuleFS(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleFileSystem";
}

bool ModuleFS::Start()
{
	mesh_file_extension = ".smesh";
	scene_file_extension = ".sscene";

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
	std::string toSavePathAssets = resources_directory + "Assets/"  + name_and_extension;
	std::string toSavePathLibraryMesh = resources_directory + "Library/" + "Meshes/" + name_and_extension;

	if (extension == "fbx")
	{
		CopyFile(path, toSavePathAssets.c_str(), true);
		LoadScene(toSavePathAssets.c_str(), direction_without_name.c_str());
	}
	else if (extension == "dds" || extension == "png")
	{
		CopyFile(path, toSavePathAssets.c_str(), true);
		materialImporter->Import(toSavePathAssets.c_str());
	}
	else if (extension == "smesh")
	{
		CopyFile(path, toSavePathLibraryMesh.c_str(), true);
		sceneImporter->LoadMesh(toSavePathLibraryMesh.c_str());
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

uuid_unit ModuleFS::CreateOwnMesh(Mesh* mesh)
{
	if (mesh == nullptr)
		return 0;

	// Header
	unsigned int ranges[4] = { mesh->GetVerticesSize(), mesh->GetIndicesSize(), mesh->GetTextureCoorSize(), mesh->GetNormalsSize() };

	// Body
	unsigned int size = sizeof(ranges) + sizeof(GLfloat) * mesh->GetVerticesSize() * 3 + sizeof(unsigned int) * mesh->GetIndicesSize() * 3 + mesh->GetTextureCoorSize() * sizeof(GLfloat) * 2 + mesh->GetNormalsSize() * sizeof(float) * 3 * 2;

	// Data & Cursor
	char* data = new char[size];	// Contenidor
	char* cursor = data;			// Apuntador

	// Add header
	unsigned int bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);

	// Put vertices
	cursor += bytes;
	bytes = sizeof(GLfloat) * mesh->GetVerticesSize() * 3;
	memcpy(cursor, mesh->verticesArray, bytes);

	// Put indices
	cursor += bytes;
	bytes = sizeof(unsigned int) * mesh->GetIndicesSize() * 3;
	memcpy(cursor, mesh->indicesArray, bytes);

	// Put texture coords
	cursor += bytes;
	bytes = sizeof(GLfloat) * mesh->GetTextureCoorSize() * 2;
	memcpy(cursor, mesh->textureCoords, bytes);

	// Put Normals
	cursor += bytes;
	bytes = sizeof(GLfloat) * 3 * 2;
	for (int i = 0; i < mesh->GetNormalsSize(); i++)
	{
		memcpy(cursor, mesh->GetNormalsVertices(i), bytes);
		cursor += bytes;
	}

	uuid_unit nUUID = App->input->GenerateUUID();
	std::string newDirection = App->fs->resources_directory + "Library/" + "Meshes/" + std::to_string(nUUID) + mesh_file_extension;

	PHYSFS_File* file = PHYSFS_openWrite(newDirection.c_str());
	if (file != nullptr)
		PHYSFS_writeBytes(file, data, size);
	PHYSFS_close(file);

	cursor = nullptr;
	delete[] data;

	return nUUID;
}

uuid_unit ModuleFS::CreateOwnTexture(Texture* texture)
{
	uuid_unit nUUID = App->input->GenerateUUID();

	return nUUID;
}

void ModuleFS::OnSaveScene(GameObject* gameObject, std::string name)
{
	JSON_Value *root_value = json_value_init_object();
	JSON_Object *root_object = json_value_get_object(root_value);
	
	//json_object_dotset_string(root_object, "address.city", "Cupertino");
	//json_object_set_string(root_object, "name", "John Smith");
	//json_object_set_number(root_object, "age", 25);
	//json_object_dotset_value(root_object, "contact.emails", json_parse_string("[\"email@example.com\",\"email2@example.com\"]"));
	
	//JSON_Array *array = json_object_get_array(root_object, "GO");
	
	JSON_Value *branch = json_value_init_array();
	JSON_Array *leaves = json_value_get_array(branch);
	JSON_Value *leaf_value = json_value_init_object();
	JSON_Object *leaf_object = json_value_get_object(leaf_value);
	json_object_set_number(leaf_object, "name1", 123);
	json_object_set_number(leaf_object, "name2", 456);
	json_object_set_number(leaf_object, "name3", 789);
	json_array_append_value(leaves, leaf_value);

	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		if (App->scene->gameObjects[i] != nullptr)
		{
			json_array_append_value(leaves, App->scene->root->GetChild(i)->OnSaveJSON());
			//json_object_dotset_value(root_object, "GameObject", App->scene->root->GetChild(i)->OnSaveJSON());
		}
	}
	root_value = json_array_get_value(leaves, App->scene->gameObjects.size());

	
	json_serialize_to_file(root_value, (App->fs->resources_directory + "Library/" + "Scenes/" + name + scene_file_extension).c_str());
	json_value_free(root_value);
}

void ModuleFS::OnLoadScene(const char* path)
{

}