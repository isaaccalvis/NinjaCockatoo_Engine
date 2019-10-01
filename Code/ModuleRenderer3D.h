#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"

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

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);

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

	// TODO, PREGUTNA, ES MOLT GUARRO TENIR LES VARIABLES EN PUBLIC ?

	bool GetWireframeMode();
	//bool GetGlDepthTest();
	//bool GetGlCullFace();
	//bool GetGlLighting();
	//bool GetGlColorMaterial();
	//bool GetGlTexture();
	//bool GetGlAlphaTest();
	//bool GetGlLineSmooth();
private:
	SDL_GLContext context;
	bool renderWireframeMode = false;

public:
	Light lights[MAX_LIGHTS];
	
	//math::float3x3 NormalMatrix;
	//math::float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool renderGlDepthTest = true;
	bool renderGlCullFace = true;
	bool renderGlLighting = true;
	bool renderGlColorMaterial = true;
	bool renderGlTexture = true;
	bool renderGlAlphaTest = false;
	bool renderGlLineSmooth = false;
};