#ifndef __ModuleFS_H__
#define __ModuleFS_H__

#include "Globals.h"
#include "Module.h"

#include "Importer.h"
#include "SceneImporter.h"
#include "MaterialImporter.h"

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
	void LoadScene(const char* path, const char* originalPath = nullptr);

	bool CheckExistingFolder(const char* path);
	void CreateFolder(const char* path);

	unsigned int CreateOwnMesh(Mesh* mesh);
	unsigned int CreateOwnTexture(Texture* texture);
	unsigned int CreateScene(GameObject* gameObject);

public:
	std::string resources_directory;
	std::string mesh_file_extension;
	std::string scene_file_extension;

	SceneImporter* sceneImporter = nullptr;
	MaterialImporter* materialImporter = nullptr;
};

#endif