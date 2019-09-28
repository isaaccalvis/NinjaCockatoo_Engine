#include "Application.h"
#include "ModuleTextures.h"

#include "mmgr/mmgr.h"

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

update_status ModuleTextures::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleTextures::CleanUp()
{
	textures.clear();

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

void ModuleTextures::AddTexture(Texture* texture)
{
	if (texture != nullptr)
	{
		textures.push_back(texture);
		// TODO: Treure que el last texture sigui assignat aqui
		last_texture = texture;
	}
}

void ModuleTextures::DeleteTexture(Texture* texture)
{
	for (std::list<Texture*>::iterator item = textures.end(); item != textures.begin();)
	{
		if ((*item) == texture)
		{
			item--;
			textures.erase(item++);
		}
	}
}