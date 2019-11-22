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

	ImGui::Text("Materials");

	ImGui::End();
}