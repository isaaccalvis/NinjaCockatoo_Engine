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
	LoadModulesInfo();
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		ret = (*item)->Init(json_object_get_object(root_object, (*item)->name));
	}

	if (root_value) { json_value_free(root_value); }


	LOG("Application Start --------------");
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		ret = (*item)->Start();
	}
	
	ms_timer.Start();
	return ret;
}

void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
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
	JSON_Value* root_value = json_parse_file("Resources/config.json");
	JSON_Object* root_object = json_value_get_object(root_value);
	char *serialized_string = NULL;

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; item++)
	{
		ret = (*item)->Save(json_object_get_object(root_object, (*item)->name));
	}
	json_serialize_to_file(root_value, "Resources/config.json");

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
	JSON_Value* root_value = json_parse_file("Resources/config.json");
	JSON_Object* root_object = json_value_get_object(root_value);
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; item++)
	{
		ret = (*item)->Load(json_object_get_object(root_object, (*item)->name));
	}
	if (root_value) { json_value_free(root_value); }
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}