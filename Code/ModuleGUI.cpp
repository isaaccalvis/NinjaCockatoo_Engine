#include "Application.h"
#include "ModuleGUI.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"

#include "GUI_About.h"
#include "GUI_Configuration.h"
#include "GUI_Properties.h"
#include "GUI_TopBar.h"

#include "Console.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleGUI";
}

bool ModuleGUI::Start()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer3D->GetContext());
	ImGui_ImplOpenGL2_Init();

	SDL_GetVersion(&sdl_version);

	// Init Windows
	GUI_Panel* gui_topbar = new GUI_TopBar();
	guiPanels.push_back(gui_topbar);
	GUI_Panel* gui_configuration = new GUI_Configuration(SDL_SCANCODE_F1);
	guiPanels.push_back(gui_configuration);
	GUI_Panel* gui_properties = new GUI_Properties(SDL_SCANCODE_F2);
	guiPanels.push_back(gui_properties);
	GUI_Panel* gui_about = new GUI_About(SDL_SCANCODE_F9);
	guiPanels.push_back(gui_about);
	return true;
}

update_status ModuleGUI::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
	ImGui::NewFrame();

	for (std::list<GUI_Panel*>::iterator it = guiPanels.begin(); it != guiPanels.end(); it++)
	{
		// Check if shortcut is pressed
		if (App->input->GetKey((*it)->GetShortCut()) == KEY_STATE::KEY_DOWN)
		{
			(*it)->SwitchActive();
		}

		// Draw
		if ((*it)->IsActive())
		{
			(*it)->Draw();
		}
	}

	// Console
	if (App->console->IsActive())
		App->console->Draw("Console", &App->console->p_open);

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

bool ModuleGUI::Save(JSON_Object* root_object)
{
	return true;
}

bool ModuleGUI::Load(JSON_Object* root_object)
{
	return true;
}

GUI_Panel* ModuleGUI::GetGUIPanel(GUI_WINDOWS type)
{
	for (std::list<GUI_Panel*>::iterator it = guiPanels.begin(); it != guiPanels.end(); it++)
	{
		if ((*it)->GetType() == type)
			return (*it);
	}
	return nullptr;
}