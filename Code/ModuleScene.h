#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include <vector>
#include "Globals.h"
#include "Module.h"
#include "GameObject.h"
#include "QuadTree.h"

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool			Start();
	update_status	Update(float dt);
	bool			CleanUp();

	void			DeleteGameObject(GameObject* go);
	GameObject*		CreateGameObject(const char* name, GameObject* parent = nullptr);
	GameObject*		SearchGameObject(std::string name);
	void			MakeCameraLookThisGOSelected();

	void MousePicking(int coor_x, int coor_y);

public: // TODO: THIS COULD BE PRIVATE
	GameObject*		root		= nullptr;
	GameObject*		camera		= nullptr;
	GameObject*		goSelected	= nullptr;
	QuadTree_d*		quadTree	= nullptr;
	std::vector<GameObject*> gameObjects;

};

#endif