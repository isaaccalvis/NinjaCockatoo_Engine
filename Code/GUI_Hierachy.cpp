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

			DragAndDrop(child);
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
			
			DragAndDrop(child);
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
		App->scene->goSelected = go;
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

void GUI_Hierachy::DragAndDrop(GameObject* go) const
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("GameObject", &go, sizeof(GameObject));
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			GameObject* target = *(GameObject**)payload->Data;

			if (target->GetParent() != go)
			{
				math::float4x4 globalMatrix = target->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->GetGlobalMatrix();
				target->GetParent()->QuitChildFromVector(target);
				go->AddChildren(target);
				target->SetParent(go);

				target->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->SetMatrixFromGlobal(globalMatrix);
			}
		}
		ImGui::EndDragDropTarget();
	}
}