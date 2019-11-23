#ifndef __Importer_H__
#define __Importer_H__

#include "Globals.h"

struct ImporterSettings {};

class Importer
{
public:
	Importer() {};
	~Importer() {};

	virtual void Import(const char* path, const uuid_unit uuid , const ImporterSettings* settings = nullptr) = 0;
};

#endif