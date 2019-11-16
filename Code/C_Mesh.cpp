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
		if (parent->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial() != nullptr)
		{
			mesh->Render(parent->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->texture, parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalMatrix);
		}
		else
		{
			mesh->Render(nullptr, parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalMatrix);
		}
	}
}

void C_Mesh::OnSaveJson(JSON_Object* object)
{

}

void C_Mesh::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
	parent->UpdateAABB();
}

Mesh* C_Mesh::GetMesh()
{
	return mesh;
}