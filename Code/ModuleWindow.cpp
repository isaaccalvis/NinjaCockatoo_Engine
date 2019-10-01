#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "mmgr/mmgr.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleWindow";
}

ModuleWindow::~ModuleWindow()
{}

bool ModuleWindow::Init(JSON_Object* root_object)
{
	LOG_IDE("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG_IDE("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = App->window->screenWidth * App->window->screenSize;
		int height = App->window->screenHeight * App->window->screenSize;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(App->window->winFullScreen)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(App->window->winResizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(App->window->winBorderless)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(App->window->winFullScreenDesktop)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		printf_s("%s\n", winTitle);
		window = SDL_CreateWindow(App->window->winTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == nullptr)
		{
			LOG_IDE("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	maxScreenWidth = DM.w;
	maxScreenHeight = DM.h;
	refreshRate = DM.refresh_rate;

	return ret;
}

update_status ModuleWindow::Update(float dt)
{
	// TODO: FICAR EL BRIGHTNESS AQUI
	return update_status::UPDATE_CONTINUE;
}

bool ModuleWindow::CleanUp()
{
	LOG_IDE("Destroying SDL window and quitting all SDL systems");

	if(window != nullptr)
	{
		SDL_DestroyWindow(window);
	}

	SDL_Quit();
	return true;
}

bool ModuleWindow::Save(JSON_Object* root_object)
{
	json_object_set_number(root_object, "screen_width", screenWidth);
	json_object_set_number(root_object, "screen_height", screenHeight);
	json_object_set_number(root_object, "screen_size", screenSize);
	json_object_set_boolean(root_object, "fullscreen", winFullScreen);
	json_object_set_boolean(root_object, "resizable", winResizable);
	json_object_set_boolean(root_object, "borderless", winBorderless);
	json_object_set_boolean(root_object, "fulldesktop", winFullScreenDesktop);
	json_object_set_boolean(root_object, "vsync", winVsync);
	json_object_set_string(root_object, "title", winTitle);
	json_object_set_string(root_object, "organization", organization);

	return true;
}

bool ModuleWindow::Load(JSON_Object* root_object)
{
	screenWidth = (int)json_object_get_number(root_object, "screen_width");
	screenHeight = (int)json_object_get_number(root_object, "screen_height");
	screenSize = (int)json_object_get_number(root_object, "screen_size");
	winFullScreen = json_object_get_boolean(root_object, "fullscreen");
	winResizable = json_object_get_boolean(root_object, "resizable");
	winBorderless = json_object_get_boolean(root_object, "borderless");
	winFullScreenDesktop = json_object_get_boolean(root_object, "fulldesktop");
	winVsync = json_object_get_boolean(root_object, "vsync");
	winTitle = strdup(json_object_get_string(root_object, "title"));
	organization = strdup(json_object_get_string(root_object, "organization"));
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	winTitle = strdup(title);
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetOrganization(const char* organization)
{
	this->organization = strdup(organization);
}

bool ModuleWindow::SetWindowsSize(int width, int height, int size)
{
	bool ret = true;
	if (height == 0)
	{
		SDL_SetWindowSize(window, screenWidth, screenHeight);
	}
	else
	{
		if (width >= 0 && height >= 0)
		{
			SDL_SetWindowSize(window, width, height);
		}
		else
		{
			LOG_IDE("Can't resize screen, width or height are < 0");
			ret = false;
		}
	}
	return ret;
}

void ModuleWindow::SetVSync(bool set)
{
	winVsync = set;
}

bool ModuleWindow::SetScreenBrightness(float brightness)
{
	bool ret = true;
	winBrightness = brightness;
	if (SDL_SetWindowBrightness(window, brightness) < 0)
	{
		LOG_IDE("Can't SetWindowBrightness");
		ret = false;
	}
	return ret;
}

bool ModuleWindow::SetFullScreen(bool set)
{
	bool ret = true;
	if (set)
	{
		if (SDL_SetWindowFullscreen(window, 1) < 0)
		{
			LOG_IDE("Can't SetWindowFullscreen");
			ret = false;
		}
	}
	else
	{
		if (SDL_SetWindowFullscreen(window, 0) < 0)
		{
			LOG_IDE("Can't SetWindowFullscreen");
			ret = false;
		}
	}
	return ret;
}

bool ModuleWindow::SetFullScreenDesktop(bool set)
{
	bool ret = true;
	if (!winFullScreen)
	{
		if (set)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
			{
				LOG_IDE("Can't SetWindowFullscreen desktop");
				ret = false;
			}
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) < 0)
			{
				LOG_IDE("Can't SetWindowFullscreen desktop");
				ret = false;
			}
		}
	}
	return ret;
}

bool ModuleWindow::SetResizable(bool set)
{
	//if (!winFullScreen)

	return true;
}

bool ModuleWindow::SetBorderless(bool set)
{
	if (!winFullScreen && !winFullScreenDesktop)
		if (set)
			SDL_SetWindowBordered(window, (SDL_bool)false);
		else
			SDL_SetWindowBordered(window, (SDL_bool)true);
	return true;
}

char* ModuleWindow::GetWindowTitle()
{
	return winTitle;
}

char* ModuleWindow::GetOrganization()
{
	return organization;
}

void ModuleWindow::GetWindowSize(int &width, int &height, int &size)
{
	width = this->screenWidth;
	height = this->screenHeight;
	size = this->screenSize;
}

bool ModuleWindow::GetVsync()
{
	return winVsync;
}

bool ModuleWindow::GetScreenBrightness()
{
	return winBrightness;
}

bool ModuleWindow::GetFullScreen()
{
	return winFullScreen;
}

bool ModuleWindow::GetFullScreenDesktop()
{
	return winFullScreenDesktop;
}

bool ModuleWindow::GetResizable()
{
	return winResizable;
}

bool ModuleWindow::GetBorderless()
{
	return winBorderless;
}

int ModuleWindow::GetMaxScreenWidth()
{
	return maxScreenWidth;
}

int ModuleWindow::GetMaxScreenHeight()
{
	return maxScreenHeight;
}

int ModuleWindow::GetScreenRefreshRate()
{
	return refreshRate;
}

SDL_Window* ModuleWindow::GetWindow() const
{
	if (window != nullptr)
	{
		return window;
	}
	else
	{
		LOG_IDE("Window is nullptr");
	}
}

void ModuleWindow::SetWindow(SDL_Window* nWindow)
{
	window = nWindow;
}