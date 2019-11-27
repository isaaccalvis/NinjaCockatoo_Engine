#include "Application.h"
#include "GUI_TopBar.h"

#include "ModuleRenderer3D.h"
#include "ModuleFS.h"

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
				App->gui->CloseEngineFromGui();
			}
			if (ImGui::MenuItem("SaveScene"))
			{
				saveWindowActive = !saveWindowActive;
				loadWindowActive = false;
			}
			if (ImGui::MenuItem("LoadScene"))
			{
				loadWindowActive = !loadWindowActive;
				saveWindowActive = false;
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
			if (ImGui::MenuItem("QuadTree"))
				App->gui->GetGUIPanel(GUI_WINDOWS::GUI_QUADTREE)->SwitchActive();
			if (ImGui::MenuItem("PlayGame"))
				App->gui->GetGUIPanel(GUI_WINDOWS::GUI_PLAYGAME)->SwitchActive();
			if (ImGui::MenuItem("Assets"))
				App->gui->GetGUIPanel(GUI_WINDOWS::GUI_ASSETS)->SwitchActive();

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Empty"))
			{
				App->scene->CreateGameObject("Empty", App->scene->root);
			}
			if (ImGui::MenuItem("Plane"))
			{
				uuid_unit uuid = App->input->GenerateUUID();
				GameObject* go = App->scene->CreateGameObject("Plane", App->scene->root);
				Component* compMesh = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
				compMesh->GetComponentAsMesh()->SetMesh(App->renderer3D->AddPrimitive(MESH_TYPE::PRIMITIVE_PLANE, uuid));
				App->scene->MakeCameraLookThisGOSelected();
			}
			if (ImGui::MenuItem("Cube"))
			{
				uuid_unit uuid = App->input->GenerateUUID();
				GameObject* go = App->scene->CreateGameObject("Cube", App->scene->root);
				Component* compMesh = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
				compMesh->GetComponentAsMesh()->SetMesh(App->renderer3D->AddPrimitive(MESH_TYPE::PRIMITIVE_CUBE, uuid));
				App->scene->MakeCameraLookThisGOSelected();
			}
			if (ImGui::MenuItem("Sphere"))
			{
				uuid_unit uuid = App->input->GenerateUUID();
				GameObject* go = App->scene->CreateGameObject("Sphere", App->scene->root);
				Component* compMesh = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
				compMesh->GetComponentAsMesh()->SetMesh(App->renderer3D->AddPrimitive(MESH_TYPE::PRIMITIVE_SPHERE, uuid));
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
			if (ImGui::MenuItem("Camera"))
			{
				if (App->scene->goSelected != nullptr)
				{
					App->scene->goSelected->CreateComponent(COMPONENT_TYPE::COMPONENT_CAMERA, "Camera");
				}
			}
			if (ImGui::MenuItem("Collider"))
			{
				if (App->scene->goSelected != nullptr)
				{
					App->scene->goSelected->CreateComponent(COMPONENT_TYPE::COMPONENT_COLLIDER, "Collider");
				}
			}
			if (ImGui::MenuItem("RigidBody"))
			{
				if (App->scene->goSelected != nullptr)
				{
					App->scene->goSelected->CreateComponent(COMPONENT_TYPE::COMPONENT_RIGIDBODY, "RigidBody");
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
		ImGui::Text("   ");
		ImGui::Checkbox("GUI : Automatic Position", &App->gui->automaticGUI_pos);

		ImGui::EndMainMenuBar();
	}

	static char gui_saveName[256];
	if (saveWindowActive)
	{
		ImGui::SetNextWindowSize(ImVec2(300, 70));
		ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
		ImGui::Begin("Save Scene", &saveWindowActive, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		if (ImGui::InputText("Save Name", gui_saveName, 256, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			App->fs->OnSaveScene(App->scene->root, gui_saveName);
			saveWindowActive = false;
		}
		ImGui::End();
	}

	static char gui_loadName[256];
	if (loadWindowActive)
	{
		ImGui::SetNextWindowSize(ImVec2(300, 70));
		ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
		ImGui::Begin("Load Scene", &loadWindowActive, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		if (ImGui::InputText("Load Name", gui_loadName, 256, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			App->fs->OnLoadScene(gui_loadName);
			loadWindowActive = false;
		}
		ImGui::End();
	}
}