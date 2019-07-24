#pragma once
#include "Module.h"
#include "Globals.h"

#define MAX_SNAKE 2

enum primitiveTypes {
	NONE,
	CUBE,
	SPHERE,
	CIRCUIT_RECTA,
	CIRCUIT_ROTA_FRONT_RIGHT,
	CIRCUIT_ROTA_FRONT_LEFT,
	CIRCUIT_ROTA_BACK_RIGHT,
	CIRCUIT_ROTA_BACK_LEFT,
	CIRCUIT_RECTA_LATERAL
};

enum SENSOR_STATE {
	SENSOR_ENTER,
	SENSOR_EXIT
};

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
};