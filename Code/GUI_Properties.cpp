#include "Application.h"
#include "GUI_Properties.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

#include "mmgr/mmgr.h"

GUI_Properties::GUI_Properties(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_PROPERTIES)
{}

void GUI_Properties::Draw()
{
	ImGui::Begin("Properties", &active, ImGuiWindowFlags_NoFocusOnAppearing);

	if (ImGui::CollapsingHeader("Transform"))
	{
		if (App->renderer3D->lastMesh != nullptr)
		{
			float auxPos[3] = { App->renderer3D->lastMesh->GetPosition().x, App->renderer3D->lastMesh->GetPosition().y, App->renderer3D->lastMesh->GetPosition().z };
			if (ImGui::InputFloat3("Position", auxPos))
			{
				App->renderer3D->lastMesh->SetPosition(math::float3(auxPos[0], auxPos[1], auxPos[2]));
			}
			ImGui::Text("Rotation:\t - , - , -");
			static float auxScale[3] = { App->renderer3D->lastMesh->GetScale().x, App->renderer3D->lastMesh->GetScale().y, App->renderer3D->lastMesh->GetScale().z };
			if (ImGui::InputFloat3("Scale", auxScale))
			{
				App->renderer3D->lastMesh->SetScale(math::float3(auxScale[0], auxScale[1], auxScale[2]));
			}
		}
	}
	if (ImGui::CollapsingHeader("Geometry"))
	{
		if (App->renderer3D->lastMesh != nullptr)
		{
			ImGui::Text("Triangles:\t %i", App->renderer3D->lastMesh->GetIndicesSize() / 3);
			ImGui::Text("Vertex:\t\t%i ", App->renderer3D->lastMesh->GetVerticesSize() / 3);
		}
	}
	if (ImGui::CollapsingHeader("Texture"))
	{
		if (App->textures->last_texture != nullptr)
		{
			ImGui::Text("Image (witdth/ heigh): %i , %i", App->textures->last_texture->GetWidth(), App->textures->last_texture->GetHeigh());
			ImGui::Image((void*)(intptr_t)App->textures->last_texture->GetBufferPos(), ImVec2(512, 512));
		}
		else
		{
			ImGui::Text("Image (witdth/ heigh): - , -");
		}
	}

	ImGui::End();
}