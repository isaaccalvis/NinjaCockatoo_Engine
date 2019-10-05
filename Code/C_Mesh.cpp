#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Transform.h"

C_Mesh::C_Mesh(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_MESH)
{

}

C_Mesh::~C_Mesh()
{

}

void C_Mesh::Update(float dt)
{
	mesh->Render(parent->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->texture);
	mesh->SetPosition(parent->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position);
	// rotation mesh->SetPosition(parent->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position);
	mesh->SetScale(parent->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->scale);
}