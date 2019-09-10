#pragma once

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGUI.h"
#include "ModuleHardware.h"
#include "ModuleMeshes.h"
#include "Console.h"

class Application
{
public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	bool SaveModulesInfo();
	bool LoadModulesInfo();

private:

	void AddModule(Module* mod);
	std::list<Module*> list_modules;

public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleGUI* gui;
	ModuleHardware* hardware;
	ModuleMeshes* meshes;

	Console* console;

private:
	Timer	ms_timer;
	Timer fps_timer;
	int count_frames;
	float last_frame_ms;
	int last_fps;
	int max_fps = 60;
	float max_ms;
	float	dt;

public:
	int GetLastFPS() const;
	int GetMaxFPS() const;
	float GetLastMS() const;
	void SetMaxFPS(int max);
};