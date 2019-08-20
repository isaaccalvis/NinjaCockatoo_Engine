#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleWindow";

	window = NULL;
	screen_surface = NULL;
}

ModuleWindow::~ModuleWindow()
{}

bool ModuleWindow::Init(JSON_Object* root_object)
{
	
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
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

		if(App->window->winFullScreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(App->window->winResizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(App->window->winBorderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(App->window->winFullScreenDesktop == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		printf_s("%s\n", winTitle);
		window = SDL_CreateWindow(App->window->winTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
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

	return ret;
}

bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	if(window != NULL)
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

void ModuleWindow::SetWindowsSize(int width, int height)
{
	if (height == 0)
	{
		SDL_SetWindowSize(window, screenWidth, screenHeight);
	}
	else
	{
		SDL_SetWindowSize(window, width, height);
	}
}