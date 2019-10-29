#include "ModuleFS.h"

ModuleFS::ModuleFS(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

bool ModuleFS::Start()
{
	return true;
}

update_status ModuleFS::PostUpdate(float dt)
{
	return update_status::UPDATE_CONTINUE;
}
bool ModuleFS::CleanUp()
{
	return true;
}

bool ModuleFS::Save(JSON_Object* root_object)
{

	return true;
}

bool ModuleFS::Load(JSON_Object* root_object)
{

	return true;
}