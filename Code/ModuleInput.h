#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"

#include "pcg-c-0.94/include/pcg_variants.h"
#include "pcg-c-0.94/extras/entropy.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(Application* app, bool start_enabled = true);
	~ModuleInput();

	bool Init(JSON_Object* root_object);
	update_status PreUpdate(float dt);
	bool CleanUp();

	bool Save(JSON_Object* root_object);
	bool Load(JSON_Object* root_object);

	KEY_STATE GetKey(int id) const;
	KEY_STATE GetMouseButton(int id) const;
	int GetMouseX() const;
	int GetMouseY() const;
	int GetMouseZ() const;
	int GetMouseXMotion() const;
	int GetMouseYMotion() const;
	void LoadPCGSeed(int argc, char** argv);

private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;
	//int mouse_z_motion;

	pcg32_random_t rng;
};

#endif