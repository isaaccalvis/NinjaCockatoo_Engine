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

	ImGui::SetWindowSize(ImVec2(App->window->screenWidth / 5, App->window->screenHeight*.25), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(App->window->screenWidth - App->window->screenWidth / 5, App->window->screenHeight*.75), ImGuiCond_Once);

	if (App->gui->automaticGUI_pos)
	{
		ImGui::SetWindowSize(ImVec2(App->window->screenWidth / 5, App->window->screenHeight*.25));
		ImGui::SetWindowPos(ImVec2(App->window->screenWidth - App->window->screenWidth / 5, App->window->screenHeight*.75));
	}

	if (ImGui::Button("Generate QuadTree"))
	{
		App->scene->quadTree->GenerateQuadTree();
	}
	
	ImGui::InputInt("Max. Division", &App->scene->quadTree->maxDivisions);
	ImGui::InputInt("Bucket Size", &App->scene->quadTree->bucketSize);
	if (App->scene->quadTree->maxDivisions < 1)
		App->scene->quadTree->maxDivisions = 1;
	if (App->scene->quadTree->bucketSize < 1)
		App->scene->quadTree->bucketSize = 1;

	bool useQuadTree = App->scene->quadTree->useQuadTree;
	if (ImGui::Checkbox("Use QuadTree", &useQuadTree))
	{
		App->scene->quadTree->useQuadTree = useQuadTree;
	}

	bool renderQuadTree = App->scene->quadTree->renderQuadTree;
	if (ImGui::Checkbox("Render QuadTree", &renderQuadTree))
	{
		App->scene->quadTree->renderQuadTree = renderQuadTree;
	}

	ImGui::Text("Static Game Objects: %i", App->scene->quadTree->static_go_list.size());

	ImGui::End();
}