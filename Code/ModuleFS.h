#ifndef __ModuleFS_H__
#define __ModuleFS_H__

#include "Globals.h"
#include "Module.h"

class ModuleFS : public Module
{
public:
	ModuleFS(Application* app, bool start_enabled = true);

	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);

};

#endif