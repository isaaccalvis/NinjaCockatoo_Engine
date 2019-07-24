#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

enum type_vehicle {
	VEHICLE_CAR,
	VEHICLE_MOTOR_BIKE,
	VEHICLE_TRUCK,
	VEHICLE_TRIPODE,
	VEHICLE_STRAMBOTIC
};

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	VehicleInfo createVehicle(type_vehicle type);

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	float rotationCameraRespectVehicle = 0;
	bool drawMeshes = true;
};