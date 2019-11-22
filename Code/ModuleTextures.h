#ifndef __ModuleTextures_H__
#define __ModuleTextures_H__

#include <list>
#include "Module.h"
#include "Globals.h"

class Texture {
public:
	Texture();
	Texture(const char* name);
	~Texture();

	void SetBufferPos(unsigned int nBufferPos);
	void SetWidth(int nWidth);
	void SetHeight(int nHeigh);

	unsigned int GetBufferPos() const;
	int GetWidth() const;
	int GetHeight() const;

public:
	const char* name = nullptr;
	uuid_unit textureUUID = 0;

private:
	unsigned int BufferPos = 0u;
	int width = 0;
	int height = 0;
};

class ModuleTextures : public Module
{
public:
	ModuleTextures(Application* app, bool start_enabled = true);

	bool CleanUp();

	void AddTexture(Texture* texture);
	Texture* SearchTexture(const char* name);
	Texture* SearchTexture(uuid_unit uuid);

	void DeleteTexture(Texture* texture);
	 
public:
	std::list<Texture*> textures;
};

#endif