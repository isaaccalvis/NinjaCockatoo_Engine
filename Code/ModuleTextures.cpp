#include "Application.h"
#include "ModuleTextures.h"

Texture::Texture()
{

}

Texture::~Texture()
{

}

ModuleTextures::ModuleTextures(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleTextures";
}

bool ModuleTextures::Start()
{
	return true;
}

update_status ModuleTextures::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleTextures::CleanUp()
{
	if (last_texture != nullptr)
		delete last_texture;

	return true;
}

bool ModuleTextures::Save(JSON_Object* root_object)
{
	return true;
}

bool ModuleTextures::Load(JSON_Object* root_object)
{
	return true;
}