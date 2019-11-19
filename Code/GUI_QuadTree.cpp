#include "Application.h"
#include "GUI_QuadTree.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

GUI_QuadTree::GUI_QuadTree(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_QUADTREE)
{}

void GUI_QuadTree::Draw()
{
	ImGui::Begin("QuadTree", &active, ImGuiWindowFlags_NoFocusOnAppearing);

	ImGui::End();
}