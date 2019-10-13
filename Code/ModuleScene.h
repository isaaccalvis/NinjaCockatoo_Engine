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

	bool			Init(JSON_Object* root_object = nullptr);
	bool			CleanUp();

	void			DeleteGameObject(GameObject* go);
	GameObject*		CreateGameObject(const char* name, GameObject* parent = nullptr);
	GameObject*		SearchGameObject(std::string name);
	void			MakeCameraLookThisGOSelected();

public: // TODO: PASSAR A PRIVAT
	GameObject*		root = nullptr;
	GameObject*		goSelected = nullptr;
	std::vector<GameObject*> gameObjects;

};

#endif