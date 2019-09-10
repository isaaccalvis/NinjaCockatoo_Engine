#ifndef __ModuleMeshes_H__
#define __ModuleMeshes_H__

#include <list>
#include "Module.h"
#include "Mesh.h"

class ModuleMeshes : public Module
{
public:
	ModuleMeshes(Application* app, bool start_enabled = true);
	virtual ~ModuleMeshes();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);
	
	void AddMesh(Mesh* mesh);

private:
	std::list<Mesh*> meshes;

};

#endif