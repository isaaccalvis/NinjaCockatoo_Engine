#ifndef __SceneImporter_H__
#define __SceneImporter_H__

#include "Importer.h"
#include "GameObject.h"

#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/cimport.h"

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

	void Import(const char* path, const ImporterSettings* settings = nullptr);
	void IterateSceneLoading(const aiScene* scene, const aiNode* node, GameObject* parent, const char* originalPath);

};

#endif