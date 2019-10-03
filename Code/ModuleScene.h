#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include <vector>
#include "Globals.h"
#include "Module.h"
#include "GameObject.h"

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	update_status Update(float dt);
	bool CleanUp();

	void AddGameObject(GameObject* go);
	GameObject* CreateGameObject(std::string name, GameObject* parent = nullptr);
	void DeleteGameObject(GameObject* go);
	GameObject* SearchGameObject(std::string name);

private:
	GameObject* root = nullptr;
	std::vector<GameObject*> gameObjects;

};

#endif