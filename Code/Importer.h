#ifndef __Importer_H__
#define __Importer_H__

struct ImporterSettings {};

class Importer
{
public:
	Importer();
	~Importer();

	virtual void Import(const char* path, const ImporterSettings* settings = nullptr) = 0;
	virtual void Load(const char* exportedFile) = 0;
};

#endif