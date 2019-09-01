#include "Application.h"
#include "ModuleMeshes.h"

ModuleMeshes::ModuleMeshes(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleMeshes";
}

ModuleMeshes::~ModuleMeshes()
{}

bool ModuleMeshes::Init(JSON_Object* root_object)
{
	return true;
}

bool ModuleMeshes::CleanUp()
{
	return true;
}

bool ModuleMeshes::Save(JSON_Object* root_object)
{
	return true;
}

bool ModuleMeshes::Load(JSON_Object* root_object)
{
	return true;
}