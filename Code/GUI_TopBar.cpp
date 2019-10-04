#include "Application.h"
#include "GUI_TopBar.h"

#include "ModuleRenderer3D.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

#include "mmgr/mmgr.h"

GUI_TopBar::GUI_TopBar(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_TOPBAR)
{
	active = true;
}

void GUI_TopBar::Draw()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit"))
			{
				// TODO: FIX THAT
				//ret = false;
				App->gui->CloseEngineFromGui();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			// TODO: FIX THAT
			if (ImGui::MenuItem("Configuration"))
				App->gui->GetGUIPanel(GUI_WINDOWS::GUI_CONFIGURATION)->SwitchActive();
			if (ImGui::MenuItem("Console"))
				App->console->SwitchActive();
			if (ImGui::MenuItem("Properties"))
				App->gui->GetGUIPanel(GUI_WINDOWS::GUI_PROPERTIES)->SwitchActive();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Cube"))
			{
				App->renderer3D->AddCube();
			}
			if (ImGui::MenuItem("Plane"))
			{
				App->renderer3D->AddPlane();
			}
			if (ImGui::MenuItem("Sphere"))
			{
				App->renderer3D->AddSphere();
			}
			if (ImGui::MenuItem("Frustrum"))
			{
				App->renderer3D->AddFrustrum();
			}
			if (ImGui::MenuItem("Cylinder"))
			{
				App->renderer3D->AddCylinder();
			}
			if (ImGui::MenuItem("DebugCube"))
			{
				App->renderer3D->AddDebugCube();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/isaaccalvis/Socialist_Engine/wiki", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Download Last"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/isaaccalvis/Socialist_Engine/releases", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Report Bug"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/isaaccalvis/Socialist_Engine/issues", NULL, NULL, SW_SHOWNORMAL);
			}
			// TODO: FIX THAT
			if (ImGui::MenuItem("About"))
				App->gui->GetGUIPanel(GUI_WINDOWS::GUI_ABOUT)->SwitchActive();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}