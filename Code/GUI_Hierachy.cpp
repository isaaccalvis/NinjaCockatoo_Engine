#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "GUI_Hierachy.h"

#include "imgui\imgui.h"
#include "imgui\imgui_internal.h"

GUI_Hierachy::GUI_Hierachy(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_HIERACHY)
{

}

void GUI_Hierachy::Draw()
{
	ImGui::Begin("Hierachy", &active, ImGuiWindowFlags_NoFocusOnAppearing);
	RecursiveTakeChilds(App->scene->root);

	ImGui::End();
}

// TODO, PREGUNTA, aixo em dona problemes... i no se que fer ...

void GUI_Hierachy::RecursiveTakeChilds(GameObject* parent) const
{
	ImGuiTreeNodeFlags treeNodeFlags;

	for (int i = 0; i < parent->CountChild(); i++)
	{
		GameObject* child = parent->GetChild(i);
		char name[256];
		sprintf_s(name, 256, child->GetName());
		if (child->CountChild() != 0)
		{
			ImGui::PushID(child);
			treeNodeFlags = 0;
			treeNodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;

			if (App->scene->goSelected == child)
				treeNodeFlags |= ImGuiTreeNodeFlags_Selected;

			bool treeNodeOpened = false;
			if (ImGui::TreeNodeEx(name, treeNodeFlags))
				treeNodeOpened = true;

			GameObjectPopUp(child);

			if (ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
				App->scene->goSelected = child;

			if (treeNodeOpened)
			{
				RecursiveTakeChilds(child);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		else
		{
			ImGui::PushID(child);

			treeNodeFlags = 0;
			treeNodeFlags |= ImGuiTreeNodeFlags_Leaf;

			if (App->scene->goSelected == child)
				treeNodeFlags |= ImGuiTreeNodeFlags_Selected;

			ImGui::TreeNodeEx(name, treeNodeFlags);
			ImGui::TreePop();
			
			GameObjectPopUp(child);

			if (ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
				App->scene->goSelected = child;
			ImGui::PopID();
		}
	}
}

void GUI_Hierachy::GameObjectPopUp(GameObject* go) const
{
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::Selectable("Create Empty"))
		{
			App->scene->CreateGameObject("Empty", go);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Selectable("Delete"))
		{
			App->scene->DeleteGameObject(App->scene->goSelected);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}