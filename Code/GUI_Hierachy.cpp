#include "Application.h"
#include "GUI_Hierachy.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

GUI_Hierachy::GUI_Hierachy(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_HIERACHY)
{

}

void GUI_Hierachy::Draw()
{
	ImGui::Begin("Hierachy", &active, ImGuiWindowFlags_NoFocusOnAppearing);

	ImGui::End();

}