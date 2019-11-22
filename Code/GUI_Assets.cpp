#include "Application.h"
#include "GUI_Assets.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

GUI_Assets::GUI_Assets(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_ASSETS)
{}

void GUI_Assets::Draw()
{
	ImGui::Begin("Assets", &active, ImGuiWindowFlags_NoFocusOnAppearing);

	ImGui::Text("Meshes");
	for (int i = 0; i < App->resources->resourcesMesh.size(); i++)
	{
		ImGui::Button(std::to_string((App->resources->resourcesMesh[i]->uuid)).c_str());
	}

	ImGui::Text("Materials");
	for (int i = 0; i < App->resources->resourceMaterial.size(); i++)
	{
		ImGui::Button(std::to_string((App->resources->resourceMaterial[i]->uuid)).c_str());
	}

	ImGui::End();
}