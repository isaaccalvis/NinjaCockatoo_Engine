#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:
	ModuleWindow(Application* app, bool start_enabled = true);
	~ModuleWindow();

	bool			Init(JSON_Object* root_object);
	update_status	Update(float dt);
	bool			CleanUp();

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);

	void	SetTitle(const char* title);
	void	SetOrganization(const char* organization);
	bool	SetWindowsSize(int width = 0, int height = 0, int size = 0);
	void	SetVSync(bool set);
	bool	SetScreenBrightness(float brightness);
	bool	SetFullScreen(bool set);
	bool	SetFullScreenDesktop(bool set);
	bool	SetResizable(bool set); // NOT WORKING (TODO)
	bool	SetBorderless(bool set);

	void	GetWindowSize(int &width, int &height, int &size);
	char*	GetWindowTitle();
	char*	GetOrganization();
	bool	GetVsync();
	bool	GetScreenBrightness();
	bool	GetFullScreen();
	bool	GetFullScreenDesktop();
	bool	GetResizable();
	bool	GetBorderless();
	int		GetMaxScreenWidth();
	int		GetMaxScreenHeight();
	int		GetScreenRefreshRate();

	SDL_Window*	GetWindow() const;
	void		SetWindow(SDL_Window* nWindow);

public:
	int		screenWidth = 0;
	int		screenHeight = 0;
	int		screenSize = 1;

private:
	SDL_Window*		window = nullptr;
	SDL_Surface*	screen_surface = nullptr;

	char*	winTitle = nullptr;
	char*	organization = nullptr;

	bool	winFullScreen = false;
	bool	winResizable = false;
	bool	winBorderless = false;
	bool	winFullScreenDesktop = false;
	bool	winVsync = false;
	int		winBrightness = 255;

	int		maxScreenWidth;
	int		maxScreenHeight;
	int		refreshRate;
};

#endif