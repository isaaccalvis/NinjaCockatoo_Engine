#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = true);
};

#endif