#ifndef __ModuleInGame_H__
#define __ModuleInGame_H__

#include <list>
#include "Module.h"

class ModuleInGame : public Module
{
public:
	ModuleInGame(Application* app, bool start_enabled = true);

	bool CleanUp();

private:

};

#endif