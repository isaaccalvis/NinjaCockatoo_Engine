#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#define SIZE 3
#define SPACE_PART_CIRCUIT 4
#define RENDER_DISTANCE 30

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	bool ret = true;
	App->camera->Move(math::float3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(math::float3(0, 0, 0));

	return ret;
}

bool ModuleSceneIntro::CleanUp()
{
	return true;
}

update_status ModuleSceneIntro::Update(float dt)
{
	return UPDATE_CONTINUE;
}