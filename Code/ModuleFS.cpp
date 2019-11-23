#include "Application.h"
#include "ModuleFS.h"

ModuleFS::ModuleFS(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleFileSystem";
}

bool ModuleFS::Start()
{
	mesh_file_extension = ".smesh";
	texture_file_extension = ".dds";
	scene_file_extension = ".sscene";
	meta_file_extension = ".smeta";

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
	if (!CheckExistingFolder((resources_directory + "Library/" + "Scenes").c_str()))
		CreateFolder((resources_directory + "Library/" + "Scenes").c_str());

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
	std::string tmp_path(path);
	for (int i = 0; i < tmp_path.size(); i++)
	{
		if (tmp_path[i] == '/')
			tmp_path[i] = 92;
	}
	std::string direction_without_name(tmp_path);
	direction_without_name = direction_without_name.substr(0, direction_without_name.find_last_of(92) + 1);
	std::string name_and_extension(tmp_path);
	name_and_extension = name_and_extension.substr(name_and_extension.find_last_of(92) + 1);
	std::string name_without_extension(name_and_extension);
	name_without_extension = name_without_extension.substr(0, name_without_extension.find_last_of(46));
	std::string extension(name_and_extension);
	extension = extension.substr(extension.find_last_of('.') + 1);
	std::string toSavePathAssets = resources_directory + "Assets/"  + name_and_extension;
	std::string toSavePathLibraryMesh = resources_directory + "Library/" + "Meshes/" + name_and_extension;

	uuid_unit uuid = App->input->GenerateUUID();
	PHYSFS_sint64 date = PHYSFS_getLastModTime(toSavePathAssets.c_str());

	if (extension == "fbx" || extension == "FBX")
	{
		JSON_Value* metaValue = CheckIfMetaExist((direction_without_name + name_without_extension + meta_file_extension).c_str());
		JSON_Object* metaObj = json_value_get_object(metaValue);
		bool fileExists = (metaValue != nullptr && date == json_object_get_number(metaObj, "date"));
		if (fileExists)
		{
			uuid_unit metaSceneUUID = json_object_get_number(metaObj, "uuid");
			fileExists = OnLoadScene((resources_directory + "Library/Meshes/" + std::to_string(metaSceneUUID) + scene_file_extension).c_str(), true, false);
		}
		if (!fileExists)
		{
			CopyFile(path, toSavePathAssets.c_str(), true);
			SceneImporterSettings* settings = new SceneImporterSettings();
			settings->originalPath = direction_without_name.c_str();
			sceneImporter->Import(toSavePathAssets.c_str(), uuid, settings);
			delete settings;
			// Generate Meta
			GenerateMeta(name_without_extension.c_str(), uuid, date);
		}
	}
	else if (extension == "dds" || extension == "png" || extension == "tga")
	{
		JSON_Value* metaValue = CheckIfMetaExist((direction_without_name + name_without_extension + meta_file_extension).c_str());
		JSON_Object* metaObj = json_value_get_object(metaValue);
		bool fileExists = (metaValue != nullptr && date == json_object_get_number(metaObj, "date"));
		if (fileExists)
		{
			uuid_unit metaSceneUUID = json_object_get_number(metaObj, "uuid");
			if (materialImporter->LoadTexture((resources_directory + "Library/Materials/" + std::to_string(metaSceneUUID) + texture_file_extension).c_str(), metaSceneUUID) != nullptr)
			{
				fileExists = true;
			}
			else
			{
				fileExists = false;
			}
		}
		if (!fileExists)
		{
			CopyFile(path, toSavePathAssets.c_str(), true);
			materialImporter->Import(toSavePathAssets.c_str(), uuid);
			GenerateMeta(name_without_extension.c_str(), uuid, date);
		}
	}
	else if (extension == "sscene")
	{
		OnLoadScene((direction_without_name + name_and_extension).c_str(), true);
	}
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

void ModuleFS::GenerateMeta(const char* name, uuid_unit uuid, PHYSFS_sint64 date)
{
	std::string newDirection = std::string(resources_directory + "Assets/" + name + meta_file_extension);

	JSON_Value* value = json_value_init_object();
	JSON_Object* object = json_value_get_object(value);
	
	json_object_set_number(object,"uuid", uuid);
	json_object_set_number(object, "date", date);

	json_serialize_to_file(value, newDirection.c_str());
	json_value_free(value);
}

JSON_Value* ModuleFS::CheckIfMetaExist(const char* name)
{
	std::string file = name;
	JSON_Value* value = json_parse_file(file.c_str());
	if (value == nullptr)
		return nullptr;
	return value;
}

void ModuleFS::CreateOwnMesh(Mesh* mesh, uuid_unit uuid)
{
	if (mesh == nullptr)
		return;

	// Header
	unsigned int ranges[4] = { mesh->GetVerticesSize(), mesh->GetIndicesSize(), mesh->GetTextureCoorSize(), mesh->GetNormalsSize() };

	// Body
	unsigned int size = sizeof(ranges) + sizeof(GLfloat) * mesh->GetVerticesSize() * 3 + sizeof(unsigned int) * mesh->GetIndicesSize() * 3 + mesh->GetTextureCoorSize() * sizeof(GLfloat) + mesh->GetNormalsSize() * sizeof(float) * 3 * 2;

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
	bytes = sizeof(GLfloat) * mesh->GetTextureCoorSize();
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

void ModuleFS::OnSaveScene(GameObject* gameObject, std::string name, std::string midPath)
{
	JSON_Value *root_value = json_value_init_array();
	JSON_Array *root_array = json_value_get_array(root_value);

	gameObject->OnSaveRecursiveJson(root_array);

	json_serialize_to_file(root_value, (App->fs->resources_directory + midPath + name + scene_file_extension).c_str());
	json_value_free(root_value);
}

bool ModuleFS::OnLoadScene(const char* originalPath, const bool isFullPath, const bool deleteScene)
{
	std::string tmp_path(originalPath);
	if (!isFullPath)
		tmp_path = std::string(resources_directory + "Library/Scenes/" + originalPath);
	
	const char* path = tmp_path.c_str();
	JSON_Value* root_value = json_parse_file(path);
	if (root_value == nullptr)
		return false;
	JSON_Array* root_array = json_value_get_array(root_value);

	if (deleteScene)
		App->scene->CleanUp();

	// Load
	JSON_Object* tmp_obj;
	for (int i = 0; i < json_array_get_count(root_array); i++)
	{
		tmp_obj = json_array_get_object(root_array, i);

		GameObject* obj = App->scene->CreateGameObject("", nullptr);
		obj->SetName(json_object_get_string(tmp_obj, "Name"));
		obj->SetUUID(json_object_get_number(tmp_obj, "UUID"));
		obj->parent_uuid = json_object_get_number(tmp_obj, "ParentUUID");

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
		if (!deleteScene)
		if (App->scene->SearchGameObject(obj->parent_uuid) == nullptr)
		{
			App->scene->root->AddChildren(obj);
			obj->SetParent(App->scene->root);
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
				obj->UpdateAABB();
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
	return true;
}