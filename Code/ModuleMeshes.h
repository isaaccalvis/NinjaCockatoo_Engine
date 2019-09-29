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

	update_status Update(float dt);
	bool CleanUp();

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);
	
	void AddMesh(Mesh* mesh);
	void DeleteMesh(Mesh* mesh);
	Mesh* AddCube();
	Mesh* AddPlane();
	Mesh* AddSphere();
	Mesh* AddFrustrum();
	Mesh* AddCylinder();
	Mesh* AddDebugCube();

public: // TODO: PREGUNTA, QUE TAN GREU ES TENIR AQUESTA LLISTA EN PUBLIC ?
	std::list<Mesh*> meshes;
	Mesh* lastMesh = nullptr;

};

#endif