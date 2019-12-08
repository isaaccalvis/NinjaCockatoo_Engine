#include "Application.h"
#include "ModuleInGame.h"

#include "C_Camera.h"
#include "C_RigidBody.h"

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

		// HardCoded for Assignment 3
		// Camera Movement
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT)
		{
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position += math::float3(0,0,cameraSpeed);
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
			App->scene->camera->UpdateAABB();
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT)
		{
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position += math::float3(0, 0, -cameraSpeed);
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
			App->scene->camera->UpdateAABB();
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT)
		{
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position += math::float3(cameraSpeed, 0, 0);
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
			App->scene->camera->UpdateAABB();
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT)
		{
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position += math::float3(-cameraSpeed, 0, 0);
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
			App->scene->camera->UpdateAABB();
		}
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_STATE::KEY_REPEAT)
		{
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position += math::float3(0, cameraSpeed, 0);
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
			App->scene->camera->UpdateAABB();
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_STATE::KEY_REPEAT)
		{
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position += math::float3(0, -cameraSpeed, 0);
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
			App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
			App->scene->camera->UpdateAABB();
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_DOWN)
		{
			math::float3 camPos = App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition;
			GameObject* bullet = App->scene->CreateGameObject("_bullet", App->scene->root);
			bullet->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position = camPos;
			bullet->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
			bullet->UpdateAABB();

			bullet->CreateComponent(COMPONENT_RIGIDBODY);
			bullet->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody->applyImpulse(btVector3(0,0, bulletImpulse), btVector3(camPos.x, camPos.y, camPos.z));
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
	if (App->scene->camera != nullptr)
	{
		onGame = true;
		runingGame = true;
		timer.Start();
		App->camera->editorCameraCopy = App->camera->camera;
		App->camera->camera = App->scene->camera->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->camera;
		App->camera->isCameraEditor = false;
		App->fs->OnSaveScene(App->scene->root, "__tmpscene");
		App->physics->activePhysics = true;
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
	App->physics->activePhysics = false;
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