#include "Application.h"
#include "ModuleResources.h"

ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleResources";
}

bool ModuleResources::CleanUp()
{
	return true;
}