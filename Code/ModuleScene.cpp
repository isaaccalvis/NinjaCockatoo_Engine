#include "ModuleScene.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleScene::~ModuleScene()
{

}

bool ModuleScene::Init(JSON_Object* root_object)
{
	root = new GameObject("_root", nullptr);
	return true;
}

update_status ModuleScene::Update(float dt)
{

	return update_status::UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{

	return true;
}

void ModuleScene::AddGameObject(GameObject* go)
{
	if (go != nullptr)
	{
		gameObjects.push_back(go);
	}
}

GameObject* ModuleScene::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* go = new GameObject(name, parent);
	go->CreateComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM);
	AddGameObject(go);
	goSelected = go;
	return go;
}

void ModuleScene::DeleteGameObject(GameObject* go)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i] == go)
		{
			gameObjects.erase(gameObjects.begin() + i);
		}
	}
}

GameObject* ModuleScene::SearchGameObject(std::string name)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->name == name.c_str())
		{
			return gameObjects[i];
		}
	}
	return nullptr;
}