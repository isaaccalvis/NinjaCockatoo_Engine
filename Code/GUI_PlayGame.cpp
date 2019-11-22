#include "Application.h"
#include "GUI_PlayGame.h"

#include "ModuleInGame.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

GUI_PlayGame::GUI_PlayGame(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_PLAYGAME)
{}

void GUI_PlayGame::Draw()
{
	ImGui::Begin("PlayGame", &active, ImGuiWindowFlags_NoFocusOnAppearing);
	if (!App->ingame->onGame)
	{
		if (ImGui::Button("Play"))
		{
			LOG_CONSOLE("Starting Game...");
			App->ingame->StartGame();
		}
	}
	else
	{
		if (ImGui::Button("Stop"))
		{
			LOG_CONSOLE("Stopping Game...");
			App->ingame->StopGame();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause"))
	{
		LOG_CONSOLE("Pause Game...");
	}
	ImGui::SameLine();
	if (ImGui::Button("Tick"))
	{
		LOG_CONSOLE("Tick; advance 1 step");
	}
	ImGui::SameLine();
	// Timer
	float totalTime = App->ingame->timer.Read();
	float hours = (int)(abs(totalTime / 360 / 1000));
	float minutes = (int)(abs(totalTime / 60 / 1000 - hours * 60));
	float seconds = (int)(abs(totalTime / 1000 - minutes * 60));
	ImGui::Text("%i:%i:%i", (int)hours, (int)minutes, (int)seconds);

	// ~Timer
	ImGui::End();
}