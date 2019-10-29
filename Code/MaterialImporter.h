#ifndef __MaterialImporter_H__
#define __MaterialImporter_H__

#include "Importer.h"

class MaterialImporter : public Importer
{
public:
	MaterialImporter();
	~MaterialImporter();

	void Import(const char* path, const ImporterSettings* settings = nullptr);
	void Load(const char* exportedFile);
};

#endif