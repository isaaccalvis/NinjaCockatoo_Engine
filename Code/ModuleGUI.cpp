#include "Application.h"

#include "ModuleGUI.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"

#include "GUI_About.h"
#include "GUI_Configuration.h"
#include "GUI_Properties.h"
#include "GUI_TopBar.h"
#include "GUI_Hierachy.h"
#include "GUI_QuadTree.h"
#include "GUI_PlayGame.h"
#include "GUI_Assets.h"
#include "GUI_Physics.h"

#include "Console.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

#include "ImGuizmo/ImGuizmo.h"

ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleGUI";
}

bool ModuleGUI::Start()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer3D->GetContext());
	ImGui_ImplOpenGL2_Init();

	SDL_GetVersion(&sdl_version);

	GUI_Panel* gui_topbar = new GUI_TopBar();
	guiPanels.push_back(gui_topbar);
	GUI_Panel* gui_configuration = new GUI_Configuration(SDL_SCANCODE_F1);
	guiPanels.push_back(gui_configuration);
	GUI_Panel* gui_properties = new GUI_Properties(SDL_SCANCODE_F2);
	guiPanels.push_back(gui_properties);
	GUI_Panel* gui_hierachy = new GUI_Hierachy(SDL_SCANCODE_F4);
	guiPanels.push_back(gui_hierachy);
	GUI_Panel* gui_about = new GUI_About(SDL_SCANCODE_F9);
	guiPanels.push_back(gui_about);
	GUI_Panel* gui_quadtree = new GUI_QuadTree(SDL_SCANCODE_F5);
	guiPanels.push_back(gui_quadtree);
	GUI_Panel* gui_playgame = new GUI_PlayGame(SDL_SCANCODE_F6);
	guiPanels.push_back(gui_playgame);
	GUI_Panel* gui_assets = new GUI_Assets(SDL_SCANCODE_F7);
	guiPanels.push_back(gui_assets);
	GUI_Panel* gui_physics = new GUI_Physics(SDL_SCANCODE_F8);
	guiPanels.push_back(gui_physics);
	return true;
}

update_status ModuleGUI::PostUpdate(float dt)
{
	update_status ret = update_status::UPDATE_CONTINUE;

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	if (App->camera->isCameraEditor == true)
	{
		UpdateGuizmo();
	}
	UpdatePanels();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	if (closeEngine)
	{
		ret = update_status::UPDATE_STOP;
	}
	return ret;
}

bool ModuleGUI::CleanUp()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
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

void ModuleGUI::UpdatePanels()
{
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

	if (App->console->IsActive())
		App->console->Draw("Console", &App->console->p_open);
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_STATE::KEY_DOWN)
		App->console->SwitchActive();
}

void ModuleGUI::UpdateGuizmo()
{
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) != KEY_STATE::KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_STATE::KEY_DOWN)
		{
			if (guizmoMode == ImGuizmo::MODE::LOCAL)
			{
				guizmoMode = ImGuizmo::MODE::WORLD;
			}
			else
			{
				guizmoMode = ImGuizmo::MODE::LOCAL;
			}
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_DOWN)
		{
			guizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		}
		else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_STATE::KEY_DOWN)
		{
			guizmoOperation = ImGuizmo::OPERATION::ROTATE;
		}
		else if (App->input->GetKey(SDL_SCANCODE_R) == KEY_STATE::KEY_DOWN)
		{
			guizmoOperation = ImGuizmo::OPERATION::SCALE;
		}
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	if (guizmoOperation == ImGuizmo::OPERATION::SCALE)
		guizmoMode = ImGuizmo::MODE::LOCAL;

	if (App->scene->goSelected != nullptr)
	{
		GameObject* go = App->scene->goSelected;
		math::float4x4 viewMatrix = App->camera->camera->GetViewMatrix();
		math::float4x4 projectionMatrix = App->camera->camera->GetProjectionMatrix();
		math::float4x4 transformMatrix = go->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->GetGlobalMatrix();
		transformMatrix = transformMatrix.Transposed();

		ImGuizmo::Manipulate(viewMatrix.ptr(), projectionMatrix.ptr(), guizmoOperation, guizmoMode, transformMatrix.ptr());

		if (ImGuizmo::IsUsing())
		{
			transformMatrix = transformMatrix.Transposed();
			go->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->SetMatrixFromGlobal(transformMatrix);
			go->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
		}
	}
}

void ModuleGUI::CloseEngineFromGui()
{
	closeEngine = true;
}