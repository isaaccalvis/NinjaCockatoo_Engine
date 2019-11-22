#ifndef __ModuleResources_H__
#define __ModuleResources_H__

#include <list>
#include "Module.h"

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);

	bool CleanUp();
};

#endif