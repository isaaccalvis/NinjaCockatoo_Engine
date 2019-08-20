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

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);

	void SetTitle(const char* title);
	void SetWindowsSize(int width = 0, int height = 0);
	void SetScreenBrightness(float brightness);
	bool SetFullScreen(bool set);
	bool SetFullScreenDesktop(bool set);
	bool SetResizable(bool set); // NOT WORKING (TODO)
	bool SetBorderless(bool set);

public:
	SDL_Window* window;
	SDL_Surface* screen_surface;

	int screenWidth = 0;
	int screenHeight = 0;
	int screenSize = 1;
	bool winFullScreen = false;
	bool winResizable = false;
	bool winBorderless = false;
	bool winFullScreenDesktop = false;
	bool winVsync = false;
	char* winTitle = nullptr;
	char* organization = nullptr;

	int maxScreenWidth;
	int maxScreenHeight;
	int refreshRate;

};

#endif