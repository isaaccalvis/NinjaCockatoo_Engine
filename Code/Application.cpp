#include "Application.h"
#include "mmgr/mmgr.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	gui = new ModuleGUI(this);
	hardware = new ModuleHardware(this);
	textures = new ModuleTextures(this);
	scene = new ModuleScene(this);
	fs = new ModuleFS(this);
	resources = new ModuleResources(this);

	console = new Console(this);

	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(gui);
	AddModule(textures);
	AddModule(scene);
	AddModule(fs);
	AddModule(resources);

	AddModule(console);

	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list<Module*>::iterator item = list_modules.end(); item != list_modules.begin();)
	{
		item--;
		list_modules.erase(item++);
	}
}

bool Application::Init()
{
	bool ret = true;
	JSON_Value* root_value = json_parse_file("config.json");
	JSON_Object* root_object = json_value_get_object(root_value);
	LoadModulesInfo();
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		ret = (*item)->Init(json_object_get_object(root_object, (*item)->name));
	}

	if (root_value) { json_value_free(root_value); }


	LOG_IDE("Application Start --------------");
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		ret = (*item)->Start();
	}
	
	ms_timer.Start();
	fps_timer.Start();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	// Prepare Update ----
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();

	if ((float)fps_timer.Read() > 1000.0f)
	{
		fps_timer.Start();
		last_fps = count_frames;
		count_frames = 0;
	}
	// Prepare Update ~~~~
	
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		ret = (*item)->PreUpdate(dt);
	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		ret = (*item)->Update(dt);
	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		ret = (*item)->PostUpdate(dt);
	}

	// Finish Update ----

	count_frames++;
	last_frame_ms = (float)ms_timer.Read();

	if (max_ms > last_frame_ms && max_ms > 0)
		SDL_Delay(max_ms - last_frame_ms);

	// Finish Update ~~~~

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		ret = (*item)->CleanUp();
	}	
	return ret;
}

bool Application::SaveModulesInfo()
{
	bool ret = true;
	JSON_Value* root_value = json_parse_file("config.json");
	JSON_Object* root_object = json_value_get_object(root_value);
	char *serialized_string = NULL;

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; item++)
	{
		ret = (*item)->Save(json_object_get_object(root_object, (*item)->name));
	}
	json_serialize_to_file(root_value, "config.json");

	//serialized_string = json_serialize_to_string_pretty(root_value);
	//puts(serialized_string);
	//json_free_serialized_string(serialized_string);
	if (root_value)
	{
		json_value_free(root_value);
	}

	return ret;
}

bool Application::LoadModulesInfo()
{
	bool ret = true;
	JSON_Value* root_value = json_parse_file("config.json");
	JSON_Object* root_object = json_value_get_object(root_value);
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; item++)
	{
		ret = (*item)->Load(json_object_get_object(root_object, (*item)->name));
	}
	if (root_value)
	{
		json_value_free(root_value);
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

int  Application::GetLastFPS() const
{
	return last_fps;
}

int Application::GetMaxFPS() const
{
	return max_fps;
}

float  Application::GetLastMS() const
{
	return last_frame_ms;
}

void  Application::SetMaxFPS(int max)
{
	max_fps = max;
	if (max > 0)
		max_ms = 1000 / max;
	else
		max_ms = 0;
}

void Application::LogConsole(const char* msg)
{
	if (console != nullptr)
		console->AddLog(msg);
}