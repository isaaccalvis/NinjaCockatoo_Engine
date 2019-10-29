#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Module.h"
#include "ModuleTextures.h"
#include "Mesh.h"

#include "Importer.h"
#include "SceneImporter.h"
#include "MaterialImporter.h"

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);

	bool Start();

	void LoadTexture(const char* path);

public:
	SceneImporter* sceneImporter = nullptr;
	MaterialImporter* materialImporter = nullptr;
};

#endif