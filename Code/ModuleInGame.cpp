#include "Application.h"
#include "ModuleInGame.h"

ModuleInGame::ModuleInGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleInGame";
}

bool ModuleInGame::CleanUp()
{
	return true;
}