#include "ModuleScene.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleScene::~ModuleScene()
{

}

update_status ModuleScene::Update(float dt)
{

	return update_status::UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{

	return true;
}