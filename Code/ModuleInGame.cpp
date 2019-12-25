#include "Application.h"
#include "ModuleInGame.h"

#include "C_Camera.h"
#include "C_Mesh.h"
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
		// Camera Movement & Rotation
		math::float3 camRot = App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->rotation.ToEulerXYZ();
		math::float3 camPos = App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_STATE::KEY_REPEAT)
			{
				camRot.x += cameraRotSpeed * dt;
			}
			else
			{
				camPos += math::float3(0, 0, cameraSpeed);
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_STATE::KEY_REPEAT)
			{
				camRot.x -= cameraRotSpeed * dt;
			}
			else
			{
				camPos += math::float3(0, 0, -cameraSpeed);
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_STATE::KEY_REPEAT)
			{
				camRot.y += cameraRotSpeed * dt;
			}
			else
			{
				camPos += math::float3(cameraSpeed, 0, 0);
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_STATE::KEY_REPEAT)
			{
				camRot.y -= cameraRotSpeed * dt;
			}
			else
			{
				camPos += math::float3(-cameraSpeed, 0, 0);
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_STATE::KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_STATE::KEY_REPEAT)
			{
				camRot.z -= cameraRotSpeed * dt;
			}
			else
			{
				camPos += math::float3(0, cameraSpeed, 0);
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_STATE::KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_STATE::KEY_REPEAT)
			{
				camRot.z += cameraRotSpeed * dt;
			}
			else
			{
				camPos += math::float3(0, -cameraSpeed, 0);
			}
		}

		App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position = camPos;
		App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->rotation = math::Quat::FromEulerXYZ(camRot.x, camRot.y, camRot.z);
		App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
		App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
		App->scene->camera->UpdateAABB();

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_DOWN)
		{
			// Create Game Object
			math::float3 camPos = App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition;
			GameObject* bullet = App->scene->CreateGameObject("_bullet", App->scene->root);
			// Update Transform
			bullet->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position = camPos;
			bullet->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->rotation = math::Quat::FromEulerXYZ(camRot.x, camRot.y, camRot.z);
			bullet->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
			bullet->UpdateAABB();
			// Create Mesh
			bullet->CreateComponent(COMPONENT_MESH);
			bullet->GetComponent(COMPONENT_MESH)->GetComponentAsMesh()->SetMesh(App->renderer3D->AddPrimitive(MESH_TYPE::PRIMITIVE_SPHERE, 0));
			// Create RigidBody
			bullet->CreateComponent(COMPONENT_RIGIDBODY);
			bullet->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->SetShape(PHYSIC_PRIMITIVE::PHY_SPHERE);
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