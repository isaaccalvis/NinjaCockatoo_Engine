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
	App->resources->DeleteResourceMesh(mesh_resources_uuid);
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
		json_object_set_number(object, "m_type", (int)mesh->type);
		json_object_set_number(object, "MeshUUID", mesh_resources_uuid);
	}
}

void C_Mesh::OnLoadJson(JSON_Object* object)
{
	int num_type = json_object_get_number(object, "m_type");
	mesh_resources_uuid = json_object_get_number(object, "MeshUUID");
	MESH_TYPE m_type = (MESH_TYPE)num_type;
	switch (m_type)
	{
	case MESH_TYPE::CUSTOM_MESH:
	{
		mesh = App->fs->sceneImporter->LoadMesh(std::string(App->fs->resources_directory + "Library/Meshes/" + std::to_string(mesh_resources_uuid) + App->fs->mesh_file_extension).c_str());
		break;
	}
	default:
	{
		mesh = App->renderer3D->AddPrimitive(m_type, mesh_resources_uuid);
		break;
	}
	}
	parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
	parent->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
}

void C_Mesh::SetMesh(Mesh* mesh)
{
	this->mesh_resources_uuid = mesh->uuid;
	this->mesh = mesh;
	parent->UpdateAABB();
}

Mesh* C_Mesh::GetMesh()
{
	return mesh;
}