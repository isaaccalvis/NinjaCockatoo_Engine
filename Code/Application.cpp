#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	gui = new ModuleGUI(this);

	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(gui);

	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list<Module*>::iterator item = list_modules.end(); item != list_modules.begin(); item--)
	{
		list_modules.erase(item--);
		//delete item->data;
		//item = item->prev;
	}
}

bool Application::Init()
{
	bool ret = true;
	JSON_Value* root_value = json_parse_file("Resources/config.json");
	JSON_Object* root_object = json_value_get_object(root_value);
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		(*item)->Init(json_object_get_object(root_object, (*item)->name));
	}
	if (root_value) { json_value_free(root_value); }


	LOG("Application Start --------------");
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		(*item)->Start();
	}
	
	ms_timer.Start();
	return ret;
}

void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

void Application::FinishUpdate()
{
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		(*item)->PreUpdate(dt);
	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		(*item)->Update(dt);
	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		(*item)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		(*item)->CleanUp();
	}	
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}