#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	char title[80];
	App->window->SetTitle(title);

	//App->camera->LookAt(vec3(x_value, 1 , z_value));
	//App->camera->LookAt(vec3(App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX(), vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getY(), App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getZ()));// = App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX();

	//App->camera->Reference.z = App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX();
	//sprintf_s(title, "%.1f Km/h, checkpoint: %i loops: %i, time: %i:%.1f, Win: %i", vehicle->GetKmh(), App->scene_intro->checkpoints_index, App->scene_intro->loopsCount, App->scene_intro->minutes, App->scene_intro->seconds, App->scene_intro->winCondition);
	return UPDATE_CONTINUE;
}