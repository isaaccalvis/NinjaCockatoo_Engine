#include "C_Mesh.h"

C_Mesh::C_Mesh(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_MESH)
{

}

C_Mesh::~C_Mesh()
{

}

void C_Mesh::Update(float dt)
{
	mesh->Render();
}