#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Module.h"
#include "ModuleTextures.h"
#include "Mesh.h"

void myCallback(const char* msg, char* userData);

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);

	bool Start();
	bool CleanUp();

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);

	void DistributeObjectToLoad(const char* path);

	void LoadMesh(const char* path, const char* originalPath = nullptr);
	Texture* LoadTexture(const char* path);

	const std::string GetResourcesDirectory() const;
	void SetResourcesDirectory(const std::string str);

private:
	std::string resources_directory;

};

#endif