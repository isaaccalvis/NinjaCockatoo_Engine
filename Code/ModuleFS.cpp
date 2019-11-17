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
	texture_file_extension = ".dds";
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
	else if (extension == "sscene")
	{
		OnLoadScene((direction_without_name + name_and_extension).c_str(), true);
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

void ModuleFS::CreateOwnMesh(Mesh* mesh, uuid_unit uuid)
{
	if (mesh == nullptr)
		return;

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

	std::string newDirection = App->fs->resources_directory + "Library/" + "Meshes/" + std::to_string(uuid) + mesh_file_extension;

	PHYSFS_File* file = PHYSFS_openWrite(newDirection.c_str());
	if (file != nullptr)
		PHYSFS_writeBytes(file, data, size);
	PHYSFS_close(file);

	cursor = nullptr;
	delete[] data;
}

void ModuleFS::OnSaveScene(GameObject* gameObject, std::string name)
{
	JSON_Value *root_value = json_value_init_array();
	JSON_Array *root_array = json_value_get_array(root_value);

	App->scene->root->OnSaveRecursiveJson(root_array);

	//int size = json_serialization_size_pretty(root_value);
	//char* buf = new char[size];
	//json_serialize_to_buffer_pretty(root_value, buf, size);

	//PHYSFS_file* file = PHYSFS_openWrite((App->fs->resources_directory + "Library/" + "Scenes/" + name + scene_file_extension).c_str());
	//PHYSFS_writeBytes(file, (const void*)buf, size);

	json_serialize_to_file(root_value, (App->fs->resources_directory + "Library/" + "Scenes/" + name + scene_file_extension).c_str());
	json_value_free(root_value);
}

void ModuleFS::OnLoadScene(const char* originalPath, const bool isFullPath)
{
	std::string tmp_path(originalPath);
	if (!isFullPath)
		tmp_path = std::string(resources_directory + "Library/Scenes/" + originalPath);
	
	const char* path = tmp_path.c_str();
	JSON_Value* root_value = json_parse_file(path);
	//JSON_Object* root_object = json_value_get_object(root_value);
	JSON_Array* root_array = json_value_get_array(root_value);

	// Clean Scene

	// Load
	JSON_Object* tmp_obj;
	for (int i = 0; i < json_array_get_count(root_array); i++)
	{
		tmp_obj = json_array_get_object(root_array, i);

		GameObject* obj = App->scene->CreateGameObject("", nullptr);
		obj->SetName(json_object_get_string(tmp_obj, "Name"));
		obj->SetUUID(json_object_get_number(tmp_obj, "UUID"));
		obj->parent_uuid = json_object_get_number(tmp_obj, "ParentUUID");

		// TODO: Delete last scene
		//App->scene->DeleteGameObject(App->scene->root);

		if (std::string(obj->GetName()).compare("root") == 0)
		{
			App->scene->root = obj;
		}
		for (int i = 0; i < App->scene->gameObjects.size(); i++)
		{
			if (App->scene->gameObjects[i]->GetUUID() == obj->parent_uuid)
			{
				App->scene->gameObjects[i]->AddChildren(obj);
				obj->SetParent(App->scene->gameObjects[i]);
			}
		}
		// Load Components
		JSON_Array* array_components = json_object_get_array(tmp_obj, "Components");
		JSON_Object* c_obj;
		for (int i = 0; i < json_array_get_count(array_components); i++)
		{
			c_obj = json_array_get_object(array_components, i);
			std::string c_type = json_object_get_string(c_obj, "c_type");
			if (c_type.compare("c_transform") == 0)
			{
				obj->GetComponent(COMPONENT_TRANSFORM)->OnLoadJson(c_obj);
			}
			else if (c_type.compare("c_mesh") == 0)
			{
				Component* compMesh = obj->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
				compMesh->OnLoadJson(c_obj);
			}
			else if (c_type.compare("c_material") == 0)
			{
				Component* compMaterial = obj->CreateComponent(COMPONENT_TYPE::COMPONENT_MATERIAL, "Material");
				compMaterial->OnLoadJson(c_obj);
			}
			else if (c_type.compare("c_camera") == 0)
			{
				Component* compCamera = obj->CreateComponent(COMPONENT_TYPE::COMPONENT_CAMERA, "Camera");
				compCamera->OnLoadJson(c_obj);
			}
		}
	}
}