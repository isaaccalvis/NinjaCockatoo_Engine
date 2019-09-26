#ifndef __ModuleTextures_H__
#define __ModuleTextures_H__

#include <list>
#include "Module.h"

class Texture {
public:
	Texture();
	~Texture();

	unsigned int BufferPos = 0u;
	int width = 0;
	int heigh = 0;
};

class ModuleTextures : public Module
{
public:
	ModuleTextures(Application* app, bool start_enabled = true);

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);

public:
	Texture* last_texture = nullptr;
};

#endif