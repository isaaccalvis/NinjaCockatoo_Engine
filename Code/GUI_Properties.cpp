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
		if (App->meshes->lastMesh != nullptr)
		{
			float auxPos[3] = { App->meshes->lastMesh->GetPosition().x, App->meshes->lastMesh->GetPosition().y, App->meshes->lastMesh->GetPosition().z };
			if (ImGui::InputFloat3("Position", auxPos))
			{
				App->meshes->lastMesh->SetPosition(math::float3(auxPos[0], auxPos[1], auxPos[2]));
			}
			ImGui::Text("Rotation:\t - , - , -");
			static float auxScale[3] = { App->meshes->lastMesh->GetScale().x, App->meshes->lastMesh->GetScale().y, App->meshes->lastMesh->GetScale().z };
			if (ImGui::InputFloat3("Scale", auxScale))
			{
				App->meshes->lastMesh->SetScale(math::float3(auxScale[0], auxScale[1], auxScale[2]));
			}
		}
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