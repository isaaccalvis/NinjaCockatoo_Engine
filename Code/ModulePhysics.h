#ifndef __ModulePhysics_H__
#define __ModulePhysics_H__

#include "Module.h"

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);

	update_status Update(float dt);
	bool CleanUp();
};

#endif