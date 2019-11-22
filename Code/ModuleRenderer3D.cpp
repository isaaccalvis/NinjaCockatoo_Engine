#include "Globals.h"
#include "Application.h"

#include "ModuleRenderer3D.h"
#include "ModuleScene.h"

#include "glew-2.1.0/include/GL/glew.h"
#include "SDL\include\SDL_opengl.h"

#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Camera.h"

#include "imgui/imgui.h"

#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glew-2.1.0/libx86/Win32/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleRenderer3D";
}

ModuleRenderer3D::~ModuleRenderer3D()
{}

bool ModuleRenderer3D::Init(JSON_Object* root_object)
{
	LOG_IDE("Creating 3D Renderer context");
	bool ret = true;
	
	context = SDL_GL_CreateContext(App->window->GetWindow());
	if(context == nullptr)
	{
		LOG_IDE("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Use Vsync
		if (App->window->GetVsync() && SDL_GL_SetSwapInterval(1) < 0)
			LOG_IDE("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
		LOG_IDE("Error initializing OpenGL! %s\n", gluErrorString(error));
		ret = false;
		}
		else
		{
		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG_IDE("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		else
		{
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glClearDepth(1.0f);

			glClearColor(0.f, 0.f, 0.f, 1.f);

			error = glGetError();
			if (error != GL_NO_ERROR)
			{
				LOG_IDE("Error initializing OpenGL! %s\n", gluErrorString(error));
				ret = false;
			}
			else
			{
				GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

				lights[0].ref = GL_LIGHT0;
				lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
				lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
				lights[0].SetPos(0.0f, 0.0f, 2.5f);
				lights[0].Init();

				GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

				GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);
				lights[0].Active(true);
				glEnable(GL_LIGHTING);
				glEnable(GL_COLOR_MATERIAL);

				GLenum glewError = glewInit();
				if (glewError != GL_NO_ERROR)
				{
					ret = false;
					LOG_CONSOLE_IDE("Error opening glew %s", glewGetErrorString(glewError));
				}
			}
		}
		}
	}

	OnResize(App->window->screenWidth, App->window->screenHeight);

	grid = new Grid(2);

	return ret;
}

update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glLoadMatrixf(App->camera->camera.GetViewMatrix().ptr());
	lights[0].SetPos(App->camera->camera.frustum.pos.x, App->camera->camera.frustum.pos.y, App->camera->camera.frustum.pos.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	// Draw QuadTree_d
	App->scene->quadTree->Render(App->scene->quadTree->root);

	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		for (int e = 0; e < App->scene->gameObjects[i]->CountComponents(); e++)
		{
			// Render Meshes
			if (App->scene->gameObjects[i]->GetComponent(e)->type == COMPONENT_TYPE::COMPONENT_MESH)
			{
				if (App->scene->camera != nullptr)
				{
					if (App->scene->camera->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->frustumCulling)
					{
						if (App->scene->camera->GetComponent(COMPONENT_CAMERA)->GetComponentAsCamera()->IsInsideFrustumCulling(App->scene->gameObjects[i]))
						{
							App->scene->gameObjects[i]->GetComponent(e)->Update(dt);
						}
					}
					else
					{
						App->scene->gameObjects[i]->GetComponent(e)->Update(dt);
					}
				}
				else
				{
					App->scene->gameObjects[i]->GetComponent(e)->Update(dt);
				}
			}
			// Render Camera
			if (App->scene->gameObjects[i]->GetComponent(e)->type == COMPONENT_TYPE::COMPONENT_CAMERA)
			{
				App->scene->gameObjects[i]->GetComponent(e)->GetComponentAsCamera()->Update(dt);
			}
		}
		// Render AABB
		if (App->renderer3D->renderBoudingBox)
		{
			if (App->scene->gameObjects[i]->boundingBoxCube != nullptr)
			{
				if (App->scene->gameObjects[i]->GetComponent(COMPONENT_MESH) != nullptr)
				{
					App->scene->gameObjects[i]->boundingBoxCube->Render();
				}
			}
		}
	}

	grid->Render();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->GetWindow());
	return UPDATE_CONTINUE;
}

bool ModuleRenderer3D::CleanUp()
{
	ClearMeshes();
	LOG_IDE("Destroying 3D Renderer");
	SDL_GL_DeleteContext(context);
	return true;
}

// We save here max_fps, App don't have own space at config.json
bool ModuleRenderer3D::Save(JSON_Object* root_object)
{
	json_object_set_number(root_object, "max_fps", App->GetMaxFPS());
	return true;
}

bool ModuleRenderer3D::Load(JSON_Object* root_object)
{
	App->SetMaxFPS((int)json_object_get_number(root_object, "max_fps"));
	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf(App->camera->camera.GetProjectionMatrix().ptr());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

SDL_GLContext ModuleRenderer3D::GetContext() const
{
	if (context != nullptr)
	{
		return context;
	}
}

void ModuleRenderer3D::SetContext(SDL_GLContext nContext)
{
	context = nContext;
}

void ModuleRenderer3D::SetWireframeMode(bool set)
{
	if (set)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

// TODO: CHECK THAT THESE OPENGL OPTIONS ARE WORKING WELL

void ModuleRenderer3D::SetGlDepthTest(bool set)
{
	if (set)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void ModuleRenderer3D::SetGlCullFace(bool set)
{
	if (set)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void ModuleRenderer3D::SetGlLighting(bool set)
{
	if (set)
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}
}

void ModuleRenderer3D::SetGlColorMaterial(bool set)
{
	if (set)
	{
		glEnable(GL_COLOR_MATERIAL);
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}
}

void ModuleRenderer3D::SetGlTexture(bool set)
{
	if (set)
	{
		glEnable(GL_TEXTURE);
	}
	else
	{
		glDisable(GL_TEXTURE);
	}
}

void ModuleRenderer3D::SetGlAlphaTest(bool set)
{
	if (set)
	{
		glEnable(GL_ALPHA_TEST);
	}
	else
	{
		glDisable(GL_ALPHA_TEST);
	}
}

void ModuleRenderer3D::SetGlLineSmooth(bool set)
{
	if (set)
	{
		glEnable(GL_LINE_SMOOTH);
	}
	else
	{
		glDisable(GL_LINE_SMOOTH);
	}
}

bool ModuleRenderer3D::GetWireframeMode()
{
	return renderWireframeMode;
}
//bool GetGlDepthTest();
//bool GetGlCullFace();
//bool GetGlLighting();
//bool GetGlColorMaterial();
//bool GetGlTexture();
//bool GetGlAlphaTest();
//bool GetGlLineSmooth();

void ModuleRenderer3D::AddMesh(Mesh* mesh)
{
	if (mesh != nullptr)
	{
		meshes.push_back(mesh);
	}
}

void ModuleRenderer3D::ClearMeshes()
{
	for (std::list<Mesh*>::iterator item = meshes.begin(); item != meshes.end(); item++)
	{
		if ((*item) != nullptr)
			delete (*item);
	}
	meshes.clear();
}

void ModuleRenderer3D::DeleteMesh(Mesh* mesh)
{
	//for (std::list<Mesh*>::iterator item = meshes.begin(); item != meshes.end(); item++)
	//{
	//	if ((*item) == mesh)
	//	{
	delete mesh;
	meshes.remove(mesh);
	//	}
	//}
}

Mesh* ModuleRenderer3D::AddPrimitive(MESH_TYPE type)
{
	Mesh *tmp_mesh = new Mesh(type);
	AddMesh(tmp_mesh);
	return tmp_mesh;
}