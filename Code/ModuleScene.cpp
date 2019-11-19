#include "Application.h"
#include "ModuleScene.h"

#include "C_Mesh.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleScene";
}

ModuleScene::~ModuleScene()
{
	CleanUp();
}

bool ModuleScene::Start()
{
	root = new GameObject("root", nullptr);
	quadTree = new QuadTree_d();
	quadTree->Create(math::AABB(math::float3(-10, -10, -10), math::float3(10, 10, 10)), quadTree);
	return true;
}

update_status ModuleScene::Update(float dt)
{
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (!ImGuizmo::IsOver())
			App->input->MousePicking(App->input->GetMouseX(), App->input->GetMouseY());
	}
	return update_status::UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	// Delete All GameObjects
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects.erase(gameObjects.begin() + i);
	}
	gameObjects.clear();

	// Delete root
	if (root != nullptr)
		delete root;

	return true;
}

void ModuleScene::DeleteGameObject(GameObject* go)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i] == go)
		{
			delete gameObjects[i];
			gameObjects.erase(gameObjects.begin() + i);
		}
	}
}

GameObject* ModuleScene::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* go = new GameObject(name, parent);
	if (go != nullptr)
	{
		gameObjects.push_back(go);
	}
	goSelected = go;
	
	return go;
}

GameObject* ModuleScene::SearchGameObject(std::string name)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->GetName() == name.c_str())
		{
			return gameObjects[i];
		}
	}
	return nullptr;
}

void ModuleScene::MakeCameraLookThisGOSelected()
{
	// Set camera pos
	if (goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MESH) != nullptr)
	{
		App->camera->camera.frustum.pos.y = goSelected->boundingBox.CenterPoint().y + goSelected->boundingBox.Diagonal().y * 2;
		App->camera->camera.frustum.pos.x = goSelected->boundingBox.CenterPoint().x + goSelected->boundingBox.Diagonal().x * 2;
		App->camera->camera.frustum.pos.z = goSelected->boundingBox.CenterPoint().z + goSelected->boundingBox.Diagonal().z * 2;
	}
	else
	{
		App->camera->camera.frustum.pos.x = goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position.x + 1;
		App->camera->camera.frustum.pos.y = goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position.y + 1;
		App->camera->camera.frustum.pos.z = goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position.z + 1;
	}
	// Look
	App->camera->LookAt(goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position);

}