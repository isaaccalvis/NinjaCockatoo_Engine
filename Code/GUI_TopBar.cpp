#include "Application.h"
#include "GUI_TopBar.h"

#include "ModuleRenderer3D.h"

#include "Component.h"
#include "C_Mesh.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

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
			if (ImGui::MenuItem("Hierachy"))
				App->gui->GetGUIPanel(GUI_WINDOWS::GUI_HIERACHY)->SwitchActive();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Empty"))
			{
				App->scene->CreateGameObject("Empty", App->scene->root);
			}
			if (ImGui::MenuItem("Cube"))
			{
				GameObject* go = App->scene->CreateGameObject("Cube", App->scene->root);
				Component* compMesh = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
				compMesh->GetComponentAsMesh()->mesh = App->renderer3D->AddPrimitive(MESH_TYPE::PRIMITIVE_CUBE);
				App->scene->MakeCameraLookThisGOSelected();
			}
			if (ImGui::MenuItem("Plane"))
			{
				GameObject* go = App->scene->CreateGameObject("Plane", App->scene->root);
				Component* compMesh = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
				compMesh->GetComponentAsMesh()->mesh = App->renderer3D->AddPrimitive(MESH_TYPE::PRIMITIVE_PLANE);
				App->scene->MakeCameraLookThisGOSelected();
			}
			if (ImGui::MenuItem("Sphere"))
			{
				GameObject* go = App->scene->CreateGameObject("Sphere", App->scene->root);
				Component* compMesh = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
				compMesh->GetComponentAsMesh()->mesh = App->renderer3D->AddPrimitive(MESH_TYPE::PRIMITIVE_SPHERE);
				App->scene->MakeCameraLookThisGOSelected();
			}
			if (ImGui::MenuItem("Frustrum"))
			{
				GameObject* go = App->scene->CreateGameObject("Frustum", App->scene->root);
				Component* compMesh = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
				compMesh->GetComponentAsMesh()->mesh = App->renderer3D->AddPrimitive(MESH_TYPE::PRIMITIVE_FRUSTRUM);
				App->scene->MakeCameraLookThisGOSelected();
			}
			if (ImGui::MenuItem("Cylinder"))
			{
				GameObject* go = App->scene->CreateGameObject("Cylinder", App->scene->root);
				Component* compMesh = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
				compMesh->GetComponentAsMesh()->mesh = App->renderer3D->AddPrimitive(MESH_TYPE::PRIMITIVE_CYLINDER);
				App->scene->MakeCameraLookThisGOSelected();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Components"))
		{
			if (ImGui::MenuItem("Mesh"))
			{
				if (App->scene->goSelected != nullptr)
				{
					App->scene->goSelected->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
				}
			}
			if (ImGui::MenuItem("Material"))
			{
				if (App->scene->goSelected != nullptr)
				{
					App->scene->goSelected->CreateComponent(COMPONENT_TYPE::COMPONENT_MATERIAL, "Material");
				}
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