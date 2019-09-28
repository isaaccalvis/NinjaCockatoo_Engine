#include "Application.h"
#include "ModuleGUI.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"

#include "Console.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

#include "mmgr/mmgr.h"

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

	return true;
}

update_status ModuleGUI::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
	ImGui::NewFrame();

	if (guiWindows[GUI_TOPBAR])
		if (!GUI_TopBar())
			return update_status::UPDATE_STOP;
	if (guiWindows[GUI_ABOUT])
		if (!GUI_AboutWindow())
			return update_status::UPDATE_STOP;
	if (guiWindows[GUI_CONFIGURATION])
		if (!GUI_ConfigurationWindow())
			return update_status::UPDATE_STOP;
	if (guiWindows[GUI_CONSOLE])
		App->console->Draw("Console", &App->console->p_open);
	if (guiWindows[GUI_PROPERTIES])
		if (!GUI_PropertiesWindow())
			return update_status::UPDATE_STOP;

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