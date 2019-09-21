#include "Application.h"
#include "ModuleImporter.h"

#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Assimp/libx86/zlibstatic.lib")

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "importer";
}

bool ModuleImporter::Start()
{
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
	return true;
}

bool ModuleImporter::Load(JSON_Object* root_object)
{
	return true;
}