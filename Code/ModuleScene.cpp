#include "ModuleScene.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleScene::~ModuleScene()
{

}

bool ModuleScene::Init()
{
	root = new GameObject();
	root->SetParent(nullptr);
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

GameObject* ModuleScene::CreateGameObject(std::string name, GameObject* parent)
{
	GameObject* go = new GameObject(name, parent);
	go->CreateComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM);
	AddGameObject(go);
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