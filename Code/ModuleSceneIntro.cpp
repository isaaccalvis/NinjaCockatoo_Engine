#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"

#include "ModulePlayer.h"

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
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

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