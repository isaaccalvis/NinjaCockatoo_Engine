#include "Application.h"
#include "C_Material.h"

C_Material::C_Material(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_MATERIAL)
{
	isUnique = true;
	name = "Material";

}

C_Material::~C_Material()
{

}

void C_Material::Update(float dt)
{

}

void C_Material::OnSaveJson(JSON_Object* object)
{
	if (parent != nullptr)
	{
		json_object_set_string(object, "c_type", "c_material");
		json_object_set_number(object, "MaterialUUID", material_resources_uuid);
	}
}

void C_Material::OnLoadJson(JSON_Object* object)
{
	material_resources_uuid = json_object_get_number(object, "MaterialUUID");
	texture = App->fs->materialImporter->LoadTexture(std::string(App->fs->resources_directory + "Library/Materials/" + std::to_string(material_resources_uuid) + App->fs->texture_file_extension).c_str(), material_resources_uuid);
}

void C_Material::SetTexture(Texture* tex)
{
	if (tex != nullptr)
	{
		this->texture = tex;
		this->material_resources_uuid = tex->textureUUID;
	}
}

Texture* C_Material::GetTexture() const
{
	return texture;
}