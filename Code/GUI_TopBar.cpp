#include "Application.h"
#include "ModuleGUI.h"

#include "ModuleMeshes.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

bool ModuleGUI::GUI_TopBar()
{
	bool ret = true;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit"))
			{
				ret = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Configuration"))
				guiWindows[GUI_CONFIGURATION] = !guiWindows[GUI_CONFIGURATION];
			if (ImGui::MenuItem("Console"))
				guiWindows[GUI_CONSOLE] = !guiWindows[GUI_CONSOLE];
			if (ImGui::MenuItem("Properties"))
				guiWindows[GUI_PROPERTIES] = !guiWindows[GUI_PROPERTIES];
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Cube"))
			{
				App->meshes->AddCube();
			}
			if (ImGui::MenuItem("Plane"))
			{
				App->meshes->AddPlane();
			}
			if (ImGui::MenuItem("Sphere"))
			{
				App->meshes->AddSphere();
			}
			if (ImGui::MenuItem("Frustrum"))
			{
				App->meshes->AddFrustrum();
			}
			if (ImGui::MenuItem("Cylinder"))
			{
				App->meshes->AddCylinder();
			}
			if (ImGui::MenuItem("DebugCube"))
			{
				App->meshes->AddDebugCube();
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
			if (ImGui::MenuItem("About"))
				guiWindows[GUI_ABOUT] = !guiWindows[GUI_ABOUT];

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	return ret;
}