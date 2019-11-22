#include "Application.h"
#include "GUI_PlayGame.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

GUI_PlayGame::GUI_PlayGame(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_PLAYGAME)
{}

void GUI_PlayGame::Draw()
{
	ImGui::Begin("PlayGame", &active, ImGuiWindowFlags_NoFocusOnAppearing);
	if (ImGui::Button("Play"))
	{
		LOG_CONSOLE("Starting Game...");
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		LOG_CONSOLE("Stop Game...");
	}
	ImGui::SameLine();
	if (ImGui::Button("Tick"))
	{
		LOG_CONSOLE("Tick; advance 1 step");
	}

	ImGui::End();
}