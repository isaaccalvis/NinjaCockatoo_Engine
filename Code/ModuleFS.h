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

	void CheckAndGenerateResourcesFolders();

public:
	std::string resources_directory;
};

#endif