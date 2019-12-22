#include "Application.h"
#include "GUI_Properties.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Camera.h"
#include "C_Collider.h"
#include "C_RigidBody.h"
#include "C_PhysicsConstraint.h"

GUI_Properties::GUI_Properties(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_PROPERTIES)
{}

void GUI_Properties::Draw()
{
	ImGui::Begin("Properties", &active, ImGuiWindowFlags_NoFocusOnAppearing);
	ImGui::SetWindowSize(ImVec2(App->window->screenWidth / 5, App->window->screenHeight*.75-18.f), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(App->window->screenWidth- App->window->screenWidth / 5, 18.0f), ImGuiCond_Once);

	if (App->gui->automaticGUI_pos)
	{
		ImGui::SetWindowSize(ImVec2(App->window->screenWidth / 5, App->window->screenHeight*.75 - 18.f));
		ImGui::SetWindowPos(ImVec2(App->window->screenWidth - App->window->screenWidth / 5, 18.0f));
	}
	

	if (App->scene->goSelected != nullptr)
	{
		// Active & Static
		bool isStatic = App->scene->goSelected->GetIsStatic();
		if (ImGui::Checkbox("Is Static", &isStatic))
		{
			App->scene->goSelected->SetIsStatic(isStatic);
		}

		for (int i = 0; i < App->scene->goSelected->CountComponents(); i++)
		{
			switch (App->scene->goSelected->GetComponent(i)->type)
			{
			case COMPONENT_TYPE::COMPONENT_TRANSFORM:
			{
				if (ImGui::CollapsingHeader("Transform"))
				{
					math::float3 tmpPos = App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position;
					float auxPos[3] = { tmpPos.x, tmpPos.y, tmpPos.z };
					if (ImGui::InputFloat3("Position", auxPos))
					{
						App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position = math::float3(auxPos[0], auxPos[1], auxPos[2]);
						App->scene->goSelected->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
						App->scene->goSelected->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
						App->scene->goSelected->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->TransformModified();
						App->scene->goSelected->UpdateAABB();
					}

					math::float3 tmpRot = App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->rotation.ToEulerXYZ();
					float auxRot[3] = { tmpRot.x * RADTODEG, tmpRot.y * RADTODEG, tmpRot.z * RADTODEG };
					if (ImGui::InputFloat3("Rotation", auxRot))
					{
						App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->rotation = math::Quat::FromEulerXYZ(auxRot[0] * DEGTORAD, auxRot[1] * DEGTORAD, auxRot[2] * DEGTORAD);
						App->scene->goSelected->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
						App->scene->goSelected->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
					}

					math::float3 tmpScale = App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->scale;
					float auxScale[3] = { tmpScale.x, tmpScale.y, tmpScale.z };
					if (ImGui::InputFloat3("Scale", auxScale))
					{
						App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->scale = math::float3(auxScale[0], auxScale[1], auxScale[2]);
						App->scene->goSelected->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
						App->scene->goSelected->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
						App->scene->goSelected->UpdateAABB();
					}
				}
			}
			break;
			case COMPONENT_TYPE::COMPONENT_MESH:
			{
				if (ImGui::CollapsingHeader("Mesh"))
				{
					if (App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMesh() != nullptr)
					{
						ImGui::Text("Triangles:\t %i", App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MESH)->GetComponentAsMesh()->GetMesh()->GetIndicesSize());
						ImGui::Text("Vertex:\t\t%i ", App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MESH)->GetComponentAsMesh()->GetMesh()->GetVerticesSize());
					}
				}
			}
			break;
			case COMPONENT_TYPE::COMPONENT_MATERIAL:
			{
				if (ImGui::CollapsingHeader("Material"))
				{
					if (App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial() != nullptr)
					{
						ImGui::Text("Image (witdth/ heigh): %i , %i", App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->GetTexture()->GetWidth(), App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->GetTexture()->GetHeight());

						ImGui::Image((void*)(intptr_t)App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->GetTexture()->GetBufferPos(), ImVec2(512, 512));
					}
				}
				else
				{
					ImGui::Text("Image (witdth/ heigh): - , -");
				}
			}
			break;
			case COMPONENT_TYPE::COMPONENT_CAMERA:
			{
				if (ImGui::CollapsingHeader("Camera"))
				{
					float tmpNearPlane = App->scene->goSelected->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->camera->frustum.nearPlaneDistance;
					if (ImGui::InputFloat("NearPlane", &tmpNearPlane))
					{
						App->scene->goSelected->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->camera->frustum.nearPlaneDistance = tmpNearPlane;
					}

					float tmpFarPlane = App->scene->goSelected->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->camera->frustum.farPlaneDistance;
					if (ImGui::InputFloat("FarPlane", &tmpFarPlane))
					{
						App->scene->goSelected->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->camera->frustum.farPlaneDistance = tmpFarPlane;
					}

					float tmpVerticalFov = App->scene->goSelected->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->camera->frustum.verticalFov;
					if (ImGui::InputFloat("VerticalFov", &tmpVerticalFov))
					{
						App->scene->goSelected->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->camera->frustum.verticalFov = tmpVerticalFov;
					}

					bool isMainCamera = App->scene->goSelected->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->isMainCamera;
					if (ImGui::Checkbox("Principal Camera", &isMainCamera))
					{
						App->scene->goSelected->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->BecomeMainCamera();
					}

					bool frustumCulling = App->scene->goSelected->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->frustumCulling;
					if (ImGui::Checkbox("Frustum Culling", &frustumCulling))
					{
						App->scene->goSelected->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->frustumCulling = frustumCulling;
					}
				}
			}
			break;
			case COMPONENT_TYPE::COMPONENT_COLLIDER:
			{
				if (ImGui::CollapsingHeader("Collider"))
				{
					math::float3 tmpSize = App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_COLLIDER)->GetComponentAsCollider()->GetSize();
					float auxSize[3] = { tmpSize.x, tmpSize.y, tmpSize.z };
					if (ImGui::InputFloat3("Collider Size", auxSize))
					{
						math::float3 inputSize(auxSize[0], auxSize[1], auxSize[2]);
						App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_COLLIDER)->GetComponentAsCollider()->SetSize(inputSize);
					}

					math::float3 tmpCenter = App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_COLLIDER)->GetComponentAsCollider()->GetLocalPosition();
					float auxCenter[3] = { tmpCenter.x, tmpCenter.y, tmpCenter.z };
					if (ImGui::InputFloat3("Collider Center", auxCenter))
					{
						math::float3 inputCenter(auxCenter[0], auxCenter[1], auxCenter[2]);
						App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_COLLIDER)->GetComponentAsCollider()->SetLocalPosition(inputCenter);
					}

					static const char* shapes[] = {"None", "Cube", "Sphere"};
					const char* currentShape = App->scene->goSelected->GetComponent(COMPONENT_COLLIDER)->GetComponentAsCollider()->GetShapeString();
					static const int numTypeShapes = 3;
					if (ImGui::BeginCombo("Shape", currentShape))
					{
						for (int i = 0; i < numTypeShapes; i++)
						{
							bool is_selected = (currentShape == shapes[i]);
							if (ImGui::Selectable(shapes[i], is_selected))
							{
								currentShape = shapes[i];
								App->scene->goSelected->GetComponent(COMPONENT_COLLIDER)->GetComponentAsCollider()->SetShape(currentShape);
							}
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}

					bool isTrigger = App->scene->goSelected->GetComponent(COMPONENT_COLLIDER)->GetComponentAsCollider()->GetIsTrigger();
					if (ImGui::Checkbox("Is Trigger", &isTrigger))
					{
						App->scene->goSelected->GetComponent(COMPONENT_COLLIDER)->GetComponentAsCollider()->SetIsTrigger(isTrigger);
					}

					C_Collider* col = App->scene->goSelected->GetComponent(COMPONENT_COLLIDER)->GetComponentAsCollider();
					ImGui::Text("Colldiding with:");
					for (std::list<GameObject*>::iterator it = col->collidingGameObjects.begin(); it != col->collidingGameObjects.end(); it++)
					{
						ImGui::Text("%s", (*it)->GetName());
					}
				}
			}
			break;
			case COMPONENT_TYPE::COMPONENT_RIGIDBODY:
			{
				if (ImGui::CollapsingHeader("RigidBody"))
				{
					static const char* shapes[] = { "None", "Cube", "Sphere" };
					const char* currentShape = App->scene->goSelected->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->GetShapeString();
					static const int numTypeShapes = 3;
					if (ImGui::BeginCombo("Shape", currentShape))
					{
						for (int i = 0; i < numTypeShapes; i++)
						{
							bool is_selected = (currentShape == shapes[i]);
							if (ImGui::Selectable(shapes[i], is_selected))
							{
								currentShape = shapes[i];
								App->scene->goSelected->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->SetShape(currentShape);
							}
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}

					math::float3 tmpSize = App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->GetSize();
					float auxSize[3] = { tmpSize.x, tmpSize.y, tmpSize.z };
					if (ImGui::InputFloat3("RigidBody Size", auxSize))
					{
						math::float3 inputSize(auxSize[0], auxSize[1], auxSize[2]);
						App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->SetSize(inputSize);
					}

					float newMass = App->scene->goSelected->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->GetMass();
					if (ImGui::InputFloat("Mass", &newMass))
					{
						App->scene->goSelected->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->SetMass(newMass);
					}

					bool useGravity = App->scene->goSelected->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->GetGravity();
					if (ImGui::Checkbox("Use Gravity", &useGravity))
					{
						App->scene->goSelected->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->SetGravity(useGravity);
					}

					if (ImGui::Button("Clear Forces"))
					{
						App->scene->goSelected->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody()->ClearForces();
					}
					C_RigidBody* rb = App->scene->goSelected->GetComponent(COMPONENT_RIGIDBODY)->GetComponentAsRigidBody();
					ImGui::Text("Colldiding with:");
					for (std::list<GameObject*>::iterator it = rb->collidingGameObjects.begin(); it != rb->collidingGameObjects.end(); it++)
					{
						ImGui::Text("%s", (*it)->GetName());
					}
				}
			}
			break;
			case COMPONENT_TYPE::COMPONENT_CONSTRAINT:
			{
				if (ImGui::CollapsingHeader("PhysicsConstraint"))
				{
					static const char* constraints[] = { "Point2Point", "Hinge", "Slider" };
					static const char* currentconstraint = "Point2Point";
					static const int numTypeShapes = 3;
					if (ImGui::BeginCombo("Constraint Type", currentconstraint))
					{
						for (int i = 0; i < numTypeShapes; i++)
						{
							bool is_selected = (currentconstraint == constraints[i]);
							if (ImGui::Selectable(constraints[i], is_selected))
							{
								currentconstraint = constraints[i];
								App->scene->goSelected->GetComponent(COMPONENT_CONSTRAINT)->GetComponentAsConstraint()->SetConstraint(currentconstraint);
							}
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}

					if (ImGui::Button("Connect Game Object"))
					{
						App->scene->goSelected->GetComponent(COMPONENT_CONSTRAINT)->GetComponentAsConstraint()->tryingConnect = true;
					}
					if (App->scene->goSelected->GetComponent(COMPONENT_CONSTRAINT)->GetComponentAsConstraint()->connectedGO != nullptr)
					{
						ImGui::Text("Connected Game Object: %s", App->scene->goSelected->GetComponent(COMPONENT_CONSTRAINT)->GetComponentAsConstraint()->connectedGO->GetName());
					}
					else
					{
						ImGui::Text("Any Game Object connected");
					}
					// BodyA point
					math::float3 tmpBodyApoint = App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_CONSTRAINT)->GetComponentAsConstraint()->bodyApoint;
					float auxBodyApoint[3] = { tmpBodyApoint.x, tmpBodyApoint.y, tmpBodyApoint.z };
					if (ImGui::InputFloat3("BodyA Point", auxBodyApoint))
					{
						math::float3 inputPoint(auxBodyApoint[0], auxBodyApoint[1], auxBodyApoint[2]);
						App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_CONSTRAINT)->GetComponentAsConstraint()->SetBodyApoint(inputPoint);
					}
					// BodyB point
					math::float3 tmpBodyBpoint = App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_CONSTRAINT)->GetComponentAsConstraint()->bodyBpoint;
					float auxBodyBpoint[3] = { tmpBodyBpoint.x, tmpBodyBpoint.y, tmpBodyBpoint.z };
					if (ImGui::InputFloat3("BodyB Point", auxBodyBpoint))
					{
						math::float3 inputPoint(auxBodyBpoint[0], auxBodyBpoint[1], auxBodyBpoint[2]);
						App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_CONSTRAINT)->GetComponentAsConstraint()->SetBodyBpoint(inputPoint);
					}

					if (App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_CONSTRAINT)->GetComponentAsConstraint()->type == PHYSIC_CONSTRAINT::CONSTRAINT_HINGE)
					{
						// AxisA point
						math::float3 tmpAxisApoint = App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_CONSTRAINT)->GetComponentAsConstraint()->bodyAaxis;
						float auxAxisApoint[3] = { tmpAxisApoint.x, tmpAxisApoint.y, tmpAxisApoint.z };
						if (ImGui::InputFloat3("Axis A Point", auxAxisApoint))
						{
							math::float3 inputPoint(auxAxisApoint[0], auxAxisApoint[1], auxAxisApoint[2]);
							App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_CONSTRAINT)->GetComponentAsConstraint()->SetBodyAaxis(inputPoint);
						}
						// AxisB point
						math::float3 tmpAxisBpoint = App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_CONSTRAINT)->GetComponentAsConstraint()->bodyBaxis;
						float auxAxisBpoint[3] = { tmpAxisBpoint.x, tmpAxisBpoint.y, tmpAxisBpoint.z };
						if (ImGui::InputFloat3("Axis B point", auxAxisBpoint))
						{
							math::float3 inputPoint(auxAxisBpoint[0], auxAxisBpoint[1], auxAxisBpoint[2]);
							App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_CONSTRAINT)->GetComponentAsConstraint()->SetBodyBaxis(inputPoint);
						}

					}
				}
			}
			break;
			}
			GameComponentPopUp(App->scene->goSelected->GetComponent(i));
		}
	}
	ImGui::End();
}

void GUI_Properties::GameComponentPopUp(Component* component) const
{
	if (component->type != COMPONENT_TRANSFORM)
	{
		if (ImGui::BeginPopupContextItem(component->name, 1))
		{
			if (ImGui::Selectable("Delete"))
			{
				App->scene->goSelected->DeleteComponent(component->type);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}