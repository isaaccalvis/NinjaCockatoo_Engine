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
			mesh->Render(parent->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->GetTexture(), parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalMatrix);
		}
		else
		{
			mesh->Render(nullptr, parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->globalMatrix);
		}
	}
}

void C_Mesh::OnSaveJson(JSON_Object* object)
{
	if (parent != nullptr)
	{
		json_object_set_string(object, "c_type", "c_mesh");
		json_object_set_number(object, "MeshUUID", mesh_resources_uuid);
	}
}

void C_Mesh::OnLoadJson(JSON_Object* object)
{
	mesh_resources_uuid = json_object_get_number(object, "MeshUUID");
	mesh = App->fs->sceneImporter->LoadMesh(std::string(App->fs->resources_directory + "Library/Meshes/" + std::to_string(mesh_resources_uuid) + App->fs->mesh_file_extension).c_str());
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