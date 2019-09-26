#include "Application.h"
#include "ModuleGUI.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

#include "mmgr/mmgr.h"

bool ModuleGUI::GUI_PropertiesWindow()
{
	bool ret = true;

	ImGui::Begin("Properties", &guiWindows[GUI_WINDOWS::GUI_PROPERTIES], ImGuiWindowFlags_NoFocusOnAppearing);

	if (ImGui::CollapsingHeader("Transform"))
	{

	}
	if (ImGui::CollapsingHeader("Geometry"))
	{

	}
	if (ImGui::CollapsingHeader("Texture"))
	{
		if (App->textures->last_texture != nullptr)
		{
			ImGui::Text("Image (witdth/ heigh): %i , %i", App->textures->last_texture->width, App->textures->last_texture->heigh);
			ImGui::Image((void*)(intptr_t)App->textures->last_texture->BufferPos, ImVec2(512, 512));
		}
		else
		{
			ImGui::Text("Image (witdth/ heigh): - , -");
		}
	}

	ImGui::End();
	return ret;
}