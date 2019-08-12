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
	// Load info from config.json
	App->LoadModulesInfo();
	App->SaveModulesInfo();

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
	winTitle = json_object_get_string(root_object, "title");
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}