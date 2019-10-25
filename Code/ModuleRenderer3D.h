#ifndef __ModuleRenderer3D_H__
#define __ModuleRenderer3D_H__

#include "Globals.h"
#include "Module.h"
#include "Light.h"
#include "Mesh.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(JSON_Object* root_object);
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	SDL_GLContext GetContext() const;
	void SetContext(SDL_GLContext nContext);

	void SetWireframeMode(bool set);
	void SetGlDepthTest(bool set);
	void SetGlCullFace(bool set);
	void SetGlLighting(bool set);
	void SetGlColorMaterial(bool set);
	void SetGlTexture(bool set);
	void SetGlAlphaTest(bool set);
	void SetGlLineSmooth(bool set);



	bool GetWireframeMode();
	//bool GetGlDepthTest();
	//bool GetGlCullFace();
	//bool GetGlLighting();
	//bool GetGlColorMaterial();
	//bool GetGlTexture();
	//bool GetGlAlphaTest();
	//bool GetGlLineSmooth();
	void AddMesh(Mesh* mesh);
	void ClearMeshes();
	void DeleteMesh(Mesh* mesh);
	Mesh* AddPrimitive(MESH_TYPE type);

private:
	SDL_GLContext context;
	bool renderWireframeMode = false;

public:
	std::list<Mesh*> meshes;
	Mesh* lastMesh = nullptr;

	Light lights[MAX_LIGHTS];

	bool renderNormals = true;
	bool renderBoudingBox = true;
	
	bool renderGlDepthTest = true;
	bool renderGlCullFace = true;
	bool renderGlLighting = true;
	bool renderGlColorMaterial = true;
	bool renderGlTexture = true;
	bool renderGlAlphaTest = false;
	bool renderGlLineSmooth = false;
};

#endif