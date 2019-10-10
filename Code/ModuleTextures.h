#ifndef __ModuleTextures_H__
#define __ModuleTextures_H__

#include <list>
#include "Module.h"

class Texture {
public:
	Texture();
	~Texture();

	void SetBufferPos(unsigned int nBufferPos);
	void SetWidth(int nWidth);
	void SetHeight(int nHeigh);

	unsigned int GetBufferPos() const;
	int GetWidth() const;
	int GetHeight() const;

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
	void DeleteTexture(Texture* texture);

public:
	Texture* last_texture = nullptr;
	 
private:
	std::list<Texture*> textures;
};

#endif