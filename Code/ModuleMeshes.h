#ifndef __ModuleMeshes_H__
#define __ModuleMeshes_H__

#include "Module.h"

class ModuleMeshes : public Module
{
public:
	ModuleMeshes(Application* app, bool start_enabled = true);
	virtual ~ModuleMeshes();

	bool Init(JSON_Object* root_object);
	update_status Update(float dt);
	bool CleanUp();

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);

};

#endif