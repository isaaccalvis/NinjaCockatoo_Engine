#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:
	ModuleWindow(Application* app, bool start_enabled = true);
	virtual ~ModuleWindow();

	bool Init(JSON_Object* root_object);
	bool CleanUp();

	void SetTitle(const char* title);

public:
	SDL_Window* window;
	SDL_Surface* screen_surface;
};

#endif