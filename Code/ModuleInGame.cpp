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
		Camera* game_camera = App->scene->camera->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->camera;
		math::float3 camPos = App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position;
		
		float speed = App->camera->camera_mov_speed * dt;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) camPos += game_camera->frustum.up * speed;
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) camPos -= game_camera->frustum.up * speed;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) camPos += game_camera->frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) camPos -= game_camera->frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) camPos -= game_camera->frustum.WorldRight() * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) camPos += game_camera->frustum.WorldRight() * speed;

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			if (dx != 0 || dy != 0)
			{
				math::Quat rotationX = math::Quat::RotateAxisAngle({ 0.0f,1.0f,0.0f }, dx * DEGTORAD * App->camera->mouse_sensitivity);
				math::Quat rotationY = math::Quat::RotateAxisAngle(game_camera->frustum.WorldRight(), dy * DEGTORAD * App->camera->mouse_sensitivity);
				math::Quat finalRotation = rotationX * rotationY;

				game_camera->frustum.up = finalRotation * game_camera->frustum.up;
				game_camera->frustum.front = finalRotation * game_camera->frustum.front;

				float distance = (game_camera->frustum.pos - game_camera->frustum.pos).Length();
				game_camera->frustum.pos = game_camera->frustum.pos + (-game_camera->frustum.front * distance);
			}
		}

		math::float3 camRot = App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->rotation.ToEulerXYZ();

		App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position = camPos;
		App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
		App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
		App->scene->camera->UpdateAABB();

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_DOWN)
		{
			// Create Game Object
			math::float3 camPos = App->scene->camera->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalPosition;
			GameObject* bullet = App->scene->CreateGameObject("_bullet", App->scene->root);
			// Update Transform
			bullet->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position = float3(camPos.x + game_camera->frustum.front.x * 2, camPos.y + game_camera->frustum.front.y * 2, camPos.z + game_camera->frustum.front.z * 2);
			bullet->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->rotation = math::Quat::FromEulerXYZ(camRot.x, camRot.y, camRot.z);
			bullet->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
			bullet->UpdateAABB();
			// Create Mesh
			bullet->CreateComponent(COMPONENT_MESH);
			bullet->GetComponent(COMPONENT_MESH)->GetComponentAsMesh()->SetMesh(App->renderer3D->AddPrimitive(MESH_TYPE::PRIMITIVE_SPHERE, 0));
			// Create RigidBody
			bullet->CreateComponent(COMPONENT_RIGIDBODY);
			bullet->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->SetShape(PHYSIC_PRIMITIVE::PHY_SPHERE);
			bullet->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->rigidBody->applyImpulse(btVector3(game_camera->frustum.front.x * bulletImpulse, game_camera->frustum.front.y * bulletImpulse, game_camera->frustum.front.z * bulletImpulse), btVector3(camPos.x, camPos.y, camPos.z));
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

		App->scene->camera->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->modifyWithParentTransform = false;
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

	App->scene->camera->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->modifyWithParentTransform = true;

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