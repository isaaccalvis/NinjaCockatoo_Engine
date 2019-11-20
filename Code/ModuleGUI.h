#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include <list>
#include "Module.h"
#include "GUI_Panel.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = true);

	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();

	GUI_Panel* GetGUIPanel(GUI_WINDOWS type);

	void UpdatePanels();
	void UpdateGuizmo();

	void CloseEngineFromGui();
private:
	SDL_version sdl_version;
	std::list<GUI_Panel*> guiPanels;
	bool closeEngine = false;

	// Guizmo
	ImGuizmo::OPERATION guizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE guizmoMode = ImGuizmo::MODE::WORLD;
};

#endif