#ifndef __ModuleTextures_H__
#define __ModuleTextures_H__

#include "Module.h"

class ModuleTextures : public Module
{
public:
	ModuleTextures(Application* app, bool start_enabled = true);

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);

};

#endif