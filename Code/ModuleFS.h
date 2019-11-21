#ifndef __ModuleFS_H__
#define __ModuleFS_H__

#include "Globals.h"
#include "Module.h"

#include "Importer.h"
#include "SceneImporter.h"
#include "MaterialImporter.h"

#include "physfs/include/physfs.h"

#include <string>

class ModuleFS : public Module
{
public:
	ModuleFS(Application* app, bool start_enabled = true);

	bool Start();
	bool CleanUp();

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);

	void DistributeObjectToLoad(const char* path);

	bool CheckExistingFolder(const char* path);
	void CreateFolder(const char* path);

	void GenerateMeta(const char* name, uuid_unit uuid, PHYSFS_sint64 date);
	JSON_Value* CheckIfMetaExist(const char* name);

	void CreateOwnMesh(Mesh* mesh, uuid_unit uuid);
	void OnSaveScene(GameObject* gameObject, std::string name, std::string midPath = "Library/Scenes/");
	void OnLoadScene(const char* originalPath, const bool isFullPath = false);
public:
	std::string resources_directory;
	std::string mesh_file_extension;
	std::string texture_file_extension;
	std::string scene_file_extension;
	std::string meta_file_extension;

	SceneImporter* sceneImporter = nullptr;
	MaterialImporter* materialImporter = nullptr;
};

#endif