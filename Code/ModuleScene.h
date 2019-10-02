#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Globals.h"
#include "Module.h"

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	update_status Update(float dt);
	bool CleanUp();
};

#endif