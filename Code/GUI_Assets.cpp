#include <list>

#include "Application.h"
#include "GUI_Assets.h"

#include "Mesh.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

GUI_Assets::GUI_Assets(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_ASSETS)
{}

void GUI_Assets::Draw()
{
	ImGui::Begin("Assets", &active, ImGuiWindowFlags_NoFocusOnAppearing);
	ImGui::SetWindowPos(ImVec2(App->window->screenWidth / 5, App->window->screenHeight - App->window->screenHeight*.10), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(App->window->screenWidth*0.6f, App->window->screenHeight*.10), ImGuiCond_Once);

	if (App->gui->automaticGUI_pos)
	{
		ImGui::SetWindowPos(ImVec2(App->window->screenWidth / 5, App->window->screenHeight - App->window->screenHeight*.10));
		ImGui::SetWindowSize(ImVec2(App->window->screenWidth*0.6f, App->window->screenHeight*.10));
	}

	ImGui::Text("Meshes : %i", App->resources->resourcesMesh.size());
	for (int i = 0; i < App->resources->resourcesMesh.size(); i++)
	{
		ImGui::Button(App->resources->resourcesMesh[i]->name.c_str());
	}
	
	ImGui::Text("Materials : %i", App->resources->resourceMaterial.size());
	for (int i = 0; i < App->resources->resourceMaterial.size(); i++)
	{
		ImGui::Button(App->resources->resourceMaterial[i]->name.c_str());
	}

	ImGui::End();
}