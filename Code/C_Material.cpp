#include "C_Material.h"

C_Material::C_Material(GameObject* parent) : Component(parent, COMPONENT_TYPE::COMPONENT_MATERIAL)
{
	isUnique = true;
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
		json_object_set_number(object, "MaterialUUID", material_resources_uuid);
	}
}