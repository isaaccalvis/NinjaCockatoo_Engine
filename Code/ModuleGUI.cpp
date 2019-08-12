#include "Application.h"
#include "ModuleGUI.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleGUI";
}

bool ModuleGUI::Init(JSON_Object* root_object)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	return true;
}

update_status ModuleGUI::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	if (guiWindows[GUI_TOPBAR])
		if (!GUI_TopBar())
			return update_status::UPDATE_STOP;
	if (guiWindows[GUI_ABOUT])
		if (!GUI_AboutWindow())
			return update_status::UPDATE_STOP;

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleGUI::Save(JSON_Object* root_object)
{
	return true;
}

bool ModuleGUI::Load(JSON_Object* root_object)
{
	return true;
}

bool ModuleGUI::GUI_TopBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About"))
				guiWindows[GUI_ABOUT] = !guiWindows[GUI_ABOUT];
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	return true;
}

bool ModuleGUI::GUI_AboutWindow()
{
	ImGui::Begin("About", &guiWindows[GUI_WINDOWS::GUI_ABOUT], ImGuiWindowFlags_NoFocusOnAppearing);
	ImGui::Text("Socialist Engine is a video game engine made by students\nfrom CITM(UPC) at VideoGame Development & Design degree");
	ImGui::Text("Made by Isaac Calvis & Aitor Velez");


	ImGui::Text("License: MIT (More info at License file or at ReadMe)");
	ImGui::End();
	return true;
}