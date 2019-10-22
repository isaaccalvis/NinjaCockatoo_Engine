#include "Application.h"

#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Transform.h"

C_Mesh::C_Mesh(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_MESH)
{
	isUnique = true;
}

C_Mesh::~C_Mesh()
{
	App->renderer3D->DeleteMesh(mesh);
	mesh = nullptr;
}

void C_Mesh::Update(float dt)
{
	if (mesh != nullptr)
	{
		mesh->SetPosition(parent->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position);
		mesh->SetRotation(parent->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->rotation);
		//mesh->SetRotation(parent->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position);
		mesh->SetScale(parent->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->scale);

		if (parent->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial() != nullptr)
			mesh->Render(parent->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->texture);
		else
			mesh->Render();
	}
}