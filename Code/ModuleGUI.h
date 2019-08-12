#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"

enum GUI_WINDOWS
{
	GUI_TOPBAR = 0,
	GUI_ABOUT
};

class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = true);

	bool Init(JSON_Object* root_object);
	update_status Update(float dt);

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);

private:
	bool guiWindows[2] = { true, false };
	bool GUI_TopBar();
	bool GUI_AboutWindow();
};

#endif