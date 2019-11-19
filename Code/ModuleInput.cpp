#include <time.h>

#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"

#include "C_Mesh.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl2.h"
#include "imgui/imgui_impl_sdl.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleInput";

	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init(JSON_Object* root_object)
{
	LOG_IDE("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG_IDE("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= App->window->screenSize;
	mouse_y /= App->window->screenSize;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / App->window->screenSize;
			mouse_y = e.motion.y / App->window->screenSize;

			mouse_x_motion = e.motion.xrel / App->window->screenSize;
			mouse_y_motion = e.motion.yrel / App->window->screenSize;
			break;

			case (SDL_DROPFILE): {
				char* dropped_filedir = e.drop.file;
				App->fs->DistributeObjectToLoad(dropped_filedir);
				SDL_free(dropped_filedir);		// Free dropped_filedir memory
			}
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

bool ModuleInput::CleanUp()
{
	LOG_IDE("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

KEY_STATE ModuleInput::GetKey(int id) const
{
	return keyboard[id];
}

KEY_STATE ModuleInput::GetMouseButton(int id) const
{
	return mouse_buttons[id];
}

int ModuleInput::GetMouseX() const
{
	return mouse_x;
}

int ModuleInput::GetMouseY() const
{
	return mouse_y;
}

int ModuleInput::GetMouseZ() const
{
	return mouse_z;
}

int ModuleInput::GetMouseXMotion() const
{
	return mouse_x_motion;
}

int ModuleInput::GetMouseYMotion() const
{
	return mouse_y_motion;
}

void ModuleInput::LoadPCGSeed(int argc, char** argv)
{
	int rounds = 5;
	bool nondeterministic_seed = true;	// en teoria hauria de ser false, pero si ho poso false no s'activa la part
										// de sota, i els numeros no tenen seed i surten amb el mateix patro sempre

	++argv;
	--argc;
	if (argc > 0 && strcmp(argv[0], "-r") == 0) {
		nondeterministic_seed = true;
		++argv;
		--argc;
	}
	if (argc > 0) {
		rounds = atoi(argv[0]);
	}
	if (nondeterministic_seed) {
		// Seed with external entropy
		uint64_t seeds[2];
		entropy_getbytes((void*)seeds, sizeof(seeds));
		pcg32_srandom_r(&rng, seeds[0], seeds[1]);
	}
	else {
		// Seed with a fixed constant
		pcg32_srandom_r(&rng, 42u, 54u);
	}
}

uuid_unit ModuleInput::GenerateUUID()
{
	uint64_t seeds[2];
	entropy_getbytes((void*)seeds, sizeof(seeds));
	pcg32_srandom_r(&rng, seeds[0], seeds[1]);

	uuid_unit a = pcg_output_rxs_m_xs_32_32(rng.state);
	return a;
}

void ModuleInput::MousePicking(int coor_x, int coor_y)
{
	int w_width;
	int w_heigh;
	int w_size;
	App->window->GetWindowSize(w_width, w_heigh, w_size);

	float x = -(1.0f - (float(coor_x) * 2.0f) / w_width);
	float y = 1.0f - (float(coor_y) * 2.0f) / w_heigh;

	math::LineSegment line = App->camera->camera.frustum.UnProjectLineSegment(x,y);

	GameObject* obj = nullptr;

	std::vector<GameObject*> selectedGO;
	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		if (line.Intersects(App->scene->gameObjects[i]->boundingBox))
		{
			obj = App->scene->gameObjects[i];
			selectedGO.push_back(obj);
		}
	}
	obj = nullptr;

	float shortesDistance = FLOAT_INF;
	for (int i = 0; i < selectedGO.size(); i++)
	{
		if (selectedGO[i]->GetComponent(COMPONENT_MESH) != nullptr)
		{
			math::Triangle tmp_triangle;
			math::LineSegment localLine(line);
			localLine.Transform(selectedGO[i]->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->GetGlobalMatrix().Inverted());
			Mesh* tmp_selected_mesh = selectedGO[i]->GetComponent(COMPONENT_MESH)->GetComponentAsMesh()->GetMesh();
			for (int a = 0; a < tmp_selected_mesh->GetIndicesSize() * 3;)
			{
				unsigned int concreteIndice;
				concreteIndice = tmp_selected_mesh->indicesArray[a]; a++;
				tmp_triangle.a = tmp_selected_mesh->vectorVertex[concreteIndice];
				concreteIndice = tmp_selected_mesh->indicesArray[a]; a++;
				tmp_triangle.b = tmp_selected_mesh->vectorVertex[concreteIndice];
				concreteIndice = tmp_selected_mesh->indicesArray[a]; a++;
				tmp_triangle.c = tmp_selected_mesh->vectorVertex[concreteIndice];
				LOG_CONSOLE("T: %f %f %f", tmp_triangle.a.x, tmp_triangle.a.y, tmp_triangle.a.z);
				LOG_CONSOLE("T: %f %f %f", tmp_triangle.b.x, tmp_triangle.b.y, tmp_triangle.b.z);
				LOG_CONSOLE("T: %f %f %f", tmp_triangle.c.x, tmp_triangle.c.y, tmp_triangle.c.z);
				LOG_CONSOLE("=====");


				float tmp_distance;
				float3 tmp_intersect_point;
				if (localLine.Intersects(tmp_triangle, &tmp_distance, &tmp_intersect_point))
				{
					if (tmp_distance < shortesDistance)
					{
						shortesDistance = tmp_distance;
						obj = selectedGO[i];
					}
				}
			}
		}
	}
	if (obj != nullptr)
		App->scene->goSelected = obj;
}