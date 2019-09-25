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
	Mesh* AddCube();
	Mesh* AddPlane();
	Mesh* AddSphere();
	Mesh* AddFrustrum();
	Mesh* AddDebugCube();
	Mesh* AddCustomMesh(const char* path);

public: // TODO: s'ha de passar la llista mesh a privat
	std::list<Mesh*> meshes;

};

#endif