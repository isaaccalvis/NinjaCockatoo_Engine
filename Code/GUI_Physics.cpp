#include "Application.h"
#include "GUI_Physics.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

GUI_Physics::GUI_Physics(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_PHYSICS)
{}

void GUI_Physics::Draw()
{
	ImGui::Begin("Physics", &active, ImGuiWindowFlags_NoFocusOnAppearing);
	ImGui::SetWindowPos(ImVec2(App->window->screenWidth / 5, 18.0f), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(App->window->screenWidth*0.6f, App->window->screenHeight*.07), ImGuiCond_Once);
	if (App->gui->automaticGUI_pos)
	{
		ImGui::SetWindowPos(ImVec2(App->window->screenWidth / 5, 18.0f));
		ImGui::SetWindowSize(ImVec2(App->window->screenWidth*0.6f, App->window->screenHeight*.07));
	}

	ImGui::End();
}