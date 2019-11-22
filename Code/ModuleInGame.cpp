#include "Application.h"
#include "ModuleInGame.h"

#include "C_Camera.h"

ModuleInGame::ModuleInGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleInGame";
	timer.Stop();
}

update_status ModuleInGame::Update(float dt)
{
	if (onGame && runingGame)
	{
		logicUpdates++;
	}
	return update_status::UPDATE_CONTINUE;
}

bool ModuleInGame::CleanUp()
{
	return true;
}

void ModuleInGame::StartGame()
{
	if (App->scene->camera != nullptr)
	{
		onGame = true;
		runingGame = true;
		timer.Start();
		App->camera->editorCameraCopy = App->camera->camera;
		App->camera->camera = App->scene->camera->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->camera;
		App->camera->isCameraEditor = false;
		App->fs->OnSaveScene(App->scene->root, "__tmpscene");
	}
	else
	{
		LOG_CONSOLE("Need a MainCamera to PlayGame");
	}
}

void ModuleInGame::StopGame()
{
	onGame = false;
	runingGame = false;
	timer.Stop();

	App->camera->camera = App->camera->editorCameraCopy;
	App->camera->isCameraEditor = true;
	App->fs->OnLoadScene("__tmpscene.sscene");
}

void ModuleInGame::PauseGame()
{
	runingGame = false;
	timer.Pause();
}

void ModuleInGame::ContinueGame()
{
	runingGame = true;
	timer.Continue();
}

void ModuleInGame::TickGame()
{
	if (onGame && runingGame == false)
	{
		ContinueGame();
		Update(App->GetLastDt());
		PauseGame();
	}
}