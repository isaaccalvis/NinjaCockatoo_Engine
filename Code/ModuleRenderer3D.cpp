#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "SDL\include\SDL_opengl.h"

#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glew-2.1.0/lib/Release/Win32/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleRenderer3D";
}

ModuleRenderer3D::~ModuleRenderer3D()
{}

bool ModuleRenderer3D::Init(JSON_Object* root_object)
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	context = SDL_GL_CreateContext(App->window->GetWindow());
	if(context == nullptr)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Use Vsync
		if (App->window->winVsync && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError(); // TODO: this is part of GLU, maybe we have to delete it
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
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
				LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
				ret = false;
			}
			else
			{

				glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
				glClearDepth(1.0f);

				//Initialize clear color
				glClearColor(0.f, 0.f, 0.f, 1.f);

				//Check for error
				error = glGetError();
				if (error != GL_NO_ERROR)
				{
					LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
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

					glewInit();
				}
			}
		}
	}

	// Projection matrix for
	OnResize(App->window->screenWidth, App->window->screenWidth);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	// TODO: borrar aixo...
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(20, 20);
	glEnd();
	
	// Print Geometry

	// Print Debug Draw

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->GetWindow());
	return UPDATE_CONTINUE;
}

bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

bool ModuleRenderer3D::Save(JSON_Object* root_object)
{
	return true;
}

bool ModuleRenderer3D::Load(JSON_Object* root_object)
{
	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf(App->camera->camera.GetProjectionMatrix());

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