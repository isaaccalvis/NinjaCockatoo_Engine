#ifndef __SceneImporter_H__
#define __SceneImporter_H__

#include "Importer.h"
#include "GameObject.h"
#include "Mesh.h"

struct aiScene;
struct aiNode;

void myCallback(const char* msg, char* userData);

struct SceneImporterSettings : public ImporterSettings
{
	std::string originalPath;
};

class SceneImporter : public Importer
{
public:
	SceneImporter();
	~SceneImporter();

	void Import(const char* path, const uuid_unit uuid, const ImporterSettings* settings = nullptr);
	GameObject* IterateSceneLoading(const aiScene* scene, const aiNode* node, GameObject* parent, const char* originalPath);

	Mesh* LoadMesh(const char* exportedFile);
};

#endif