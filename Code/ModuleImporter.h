#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Module.h"

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);

	void LoadMesh(const char* path);

};

#endif