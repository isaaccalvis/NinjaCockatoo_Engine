#include "Application.h"
#include "ModuleInGame.h"

ModuleInGame::ModuleInGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleInGame";
}

update_status ModuleInGame::Update(float dt)
{
	if (onGame)
	{
		if (runingGame)
		{

		}
		else
		{

		}
	}
	return update_status::UPDATE_CONTINUE;
}

bool ModuleInGame::CleanUp()
{
	return true;
}

void ModuleInGame::StartGame()
{
	onGame = true;
	runingGame = true;
	timer.Start();
}

void ModuleInGame::StopGame()
{
	onGame = false;
	runingGame = false;
	timer.Stop();
}

void ModuleInGame::PauseGame()
{
	runingGame = false;
	timer.Stop();
}

void ModuleInGame::ContinueGame()
{
	runingGame = true;
	timer.Start();
}