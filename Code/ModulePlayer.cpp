#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	VehicleInfo car = createVehicle(VEHICLE_CAR);
	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 0, 0);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			acceleration = MAX_ACCELERATION + MAX_ACCELERATION / 2;
		else if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
			acceleration = MAX_ACCELERATION - MAX_ACCELERATION / 2;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES) {
			turn += TURN_DEGREES;
			rotationCameraRespectVehicle -= 0.05f;
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES) {
			turn -= TURN_DEGREES;
			rotationCameraRespectVehicle += 0.05f;
		}
	}
	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		acceleration = -MAX_ACCELERATION;
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		// elimina vehicle
		vehicle->info.deleteThisObject = true;
		App->physics->deleteVehiclesFromWorld();
		// refresca mapa
		system("mapa.tmx");
		App->physics->CleanConstraints();
		App->physics->CleanWorld();
		App->physics->CleanBodies();
		App->physics->CleanVehicle();
		App->physics->Start();
		App->scene_intro->CleanMeshes();
		App->scene_intro->createMap1();
		// crea vehicle
		VehicleInfo car = createVehicle(VEHICLE_CAR);
		vehicle = App->physics->AddVehicle(car);
		vehicle->SetPos(0, 0, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		if (drawMeshes == false)
			drawMeshes = true;
		else
			drawMeshes = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		App->physics->CleanWorld();
		App->physics->CleanBodies();
		App->physics->CleanVehicle();
		App->physics->CleanConstraints();
		App->physics->Start();
		App->scene_intro->CleanMeshes();
		App->scene_intro->CleanUp();
		App->scene_intro->createMap1();

		App->physics->deleteVehiclesFromWorld();
		VehicleInfo car = createVehicle(VEHICLE_CAR);
		vehicle = App->physics->AddVehicle(car);
		vehicle->SetPos(0, 0, 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
	{
		App->physics->CleanWorld();
		App->physics->CleanBodies();
		App->physics->CleanVehicle();
		App->physics->CleanConstraints();
		App->physics->Start();
		App->scene_intro->CleanMeshes();
		App->scene_intro->createMap1();

		App->physics->deleteVehiclesFromWorld();
		VehicleInfo car = createVehicle(VEHICLE_CAR);
		vehicle = App->physics->AddVehicle(car);
		vehicle->SetPos(0 ,0 ,0);
	}
	else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT) {
		App->physics->CleanWorld();
		App->physics->CleanBodies();
		App->physics->CleanVehicle();
		App->physics->CleanConstraints();
		App->physics->Start();
		App->scene_intro->CleanMeshes();
		App->scene_intro->createMap1();

		vehicle->info.deleteThisObject = true;
		App->physics->deleteVehiclesFromWorld();
		VehicleInfo car = createVehicle(VEHICLE_MOTOR_BIKE);
		vehicle = App->physics->AddVehicle(car);
		vehicle->SetPos(0, 0, 0);
	}
	else if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT) {
		App->physics->CleanWorld();
		App->physics->CleanBodies();
		App->physics->CleanVehicle();
		App->physics->CleanConstraints();
		App->physics->Start();
		App->scene_intro->CleanMeshes();
		App->scene_intro->createMap1();

		vehicle->info.deleteThisObject = true;
		App->physics->deleteVehiclesFromWorld();
		VehicleInfo car = createVehicle(VEHICLE_TRUCK);
		vehicle = App->physics->AddVehicle(car);
		vehicle->SetPos(0, 0, 0);
	}
	else if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT) {
		App->physics->CleanWorld();
		App->physics->CleanBodies();
		App->physics->CleanVehicle();
		App->physics->CleanConstraints();
		App->physics->Start();
		App->scene_intro->CleanMeshes();
		App->scene_intro->createMap1();

		vehicle->info.deleteThisObject = true;
		App->physics->deleteVehiclesFromWorld();
		VehicleInfo car = createVehicle(VEHICLE_TRIPODE);
		vehicle = App->physics->AddVehicle(car);
		vehicle->SetPos(0, 0, 0);
	}
	else if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT) {
		App->physics->CleanWorld();
		App->physics->CleanBodies();
		App->physics->CleanVehicle();
		App->physics->CleanConstraints();
		App->physics->Start();
		App->scene_intro->CleanMeshes();
		App->scene_intro->createMap1();

		vehicle->info.deleteThisObject = true;
		App->physics->deleteVehiclesFromWorld();
		VehicleInfo car = createVehicle(VEHICLE_STRAMBOTIC);
		vehicle = App->physics->AddVehicle(car);
		vehicle->SetPos(0, 0, 0);
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake); 

	vehicle->Render();

	char title[80];
	sprintf_s(title,"v: %.1f Km/h -- time: %is -- total_time: %is -- loops: %i --", vehicle->GetKmh(), (SDL_GetTicks() - App->scene_intro->getLastResetGetTicks)/ 1000 ,SDL_GetTicks() / 1000, App->scene_intro->loops);
	App->window->SetTitle(title);
	
	App->camera->Position.x = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() - 5 * App->player->vehicle->vehicle->getForwardVector().getX();
	App->camera->Position.y = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 5 * App->player->vehicle->vehicle->getUpAxis();
	App->camera->Position.z = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() - 10 * App->player->vehicle->vehicle->getForwardVector().getZ();

	float x_value = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() + 10 * App->player->vehicle->vehicle->getForwardVector().getX();
	float z_value = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() + 10 * App->player->vehicle->vehicle->getForwardVector().getZ();


	App->player->vehicle->getVec3Pos().y;
	App->camera->LookAt(vec3(x_value, 1 , z_value));
	//App->camera->LookAt(vec3(App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX(), vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getY(), App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getZ()));// = App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX();

	//App->camera->Reference.z = App->player->vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX();
	//sprintf_s(title, "%.1f Km/h, checkpoint: %i loops: %i, time: %i:%.1f, Win: %i", vehicle->GetKmh(), App->scene_intro->checkpoints_index, App->scene_intro->loopsCount, App->scene_intro->minutes, App->scene_intro->seconds, App->scene_intro->winCondition);
	return UPDATE_CONTINUE;
}

VehicleInfo ModulePlayer::createVehicle(type_vehicle type = VEHICLE_CAR) {
	VehicleInfo vehicle;
	switch (type) {
	case type_vehicle::VEHICLE_CAR:
	{
		vehicle.chassis_size.Set(2, 2, 4);
		vehicle.chassis_offset.Set(0, 1.5, 0);
		vehicle.mass = 500.0f;
		vehicle.suspensionStiffness = 15.88f;
		vehicle.suspensionCompression = 0.83f;
		vehicle.suspensionDamping = 0.88f;
		vehicle.maxSuspensionTravelCm = 1000.0f;
		vehicle.frictionSlip = 50.5;
		vehicle.maxSuspensionForce = 6000.0f;

		float connection_height = 1.2f;
		float wheel_radius = 0.6f;
		float wheel_width = 0.5f;
		float suspensionRestLength = 1.2f;

		float half_width = vehicle.chassis_size.x*0.5f;
		float half_length = vehicle.chassis_size.z*0.5f;

		vec3 direction(0, -1, 0);
		vec3 axis(-1, 0, 0);

		vehicle.num_wheels = 4;
		vehicle.wheels = new Wheel[4];

		vehicle.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
		vehicle.wheels[0].direction = direction;
		vehicle.wheels[0].axis = axis;
		vehicle.wheels[0].suspensionRestLength = suspensionRestLength;
		vehicle.wheels[0].radius = wheel_radius;
		vehicle.wheels[0].width = wheel_width;
		vehicle.wheels[0].front = true;
		vehicle.wheels[0].drive = true;
		vehicle.wheels[0].brake = false;
		vehicle.wheels[0].steering = true;

		vehicle.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
		vehicle.wheels[1].direction = direction;
		vehicle.wheels[1].axis = axis;
		vehicle.wheels[1].suspensionRestLength = suspensionRestLength;
		vehicle.wheels[1].radius = wheel_radius;
		vehicle.wheels[1].width = wheel_width;
		vehicle.wheels[1].front = true;
		vehicle.wheels[1].drive = true;
		vehicle.wheels[1].brake = false;
		vehicle.wheels[1].steering = true;

		vehicle.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
		vehicle.wheels[2].direction = direction;
		vehicle.wheels[2].axis = axis;
		vehicle.wheels[2].suspensionRestLength = suspensionRestLength;
		vehicle.wheels[2].radius = wheel_radius;
		vehicle.wheels[2].width = wheel_width;
		vehicle.wheels[2].front = false;
		vehicle.wheels[2].drive = false;
		vehicle.wheels[2].brake = true;
		vehicle.wheels[2].steering = false;

		vehicle.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
		vehicle.wheels[3].direction = direction;
		vehicle.wheels[3].axis = axis;
		vehicle.wheels[3].suspensionRestLength = suspensionRestLength;
		vehicle.wheels[3].radius = wheel_radius;
		vehicle.wheels[3].width = wheel_width;
		vehicle.wheels[3].front = false;
		vehicle.wheels[3].drive = false;
		vehicle.wheels[3].brake = true;
		vehicle.wheels[3].steering = false;
	}
	break;

	case type_vehicle::VEHICLE_MOTOR_BIKE: {
		vehicle.chassis_size.Set(1, 2, 4);
		vehicle.chassis_offset.Set(0, 1.5, 0);
		vehicle.mass = 300.0f;
		vehicle.suspensionStiffness = 15.88f;
		vehicle.suspensionCompression = 0.83f;
		vehicle.suspensionDamping = 10.88f;
		vehicle.maxSuspensionTravelCm = 100.0f;
		vehicle.frictionSlip = 50.5;
		vehicle.maxSuspensionForce = 2000.0f;

		float connection_height = 1.2f;
		float wheel_radius = 0.6f;
		float wheel_width = 0.7f;
		float suspensionRestLength = 0.9f;

		float half_width = vehicle.chassis_size.x*0.5f;
		float half_length = vehicle.chassis_size.z*0.5f;

		vec3 direction(0, -1, 0);
		vec3 axis(-1, 0, 0);

		vehicle.num_wheels = 2;
		vehicle.wheels = new Wheel[2];

		vehicle.wheels[0].connection.Set(0, connection_height, -wheel_radius - half_width);
		vehicle.wheels[0].direction = direction;
		vehicle.wheels[0].axis = axis;
		vehicle.wheels[0].suspensionRestLength = suspensionRestLength;
		vehicle.wheels[0].radius = wheel_radius;
		vehicle.wheels[0].width = wheel_width;
		vehicle.wheels[0].front = true;
		vehicle.wheels[0].drive = true;
		vehicle.wheels[0].brake = false;
		vehicle.wheels[0].steering = false;

		vehicle.wheels[1].connection.Set(0, connection_height, half_length - wheel_radius);
		vehicle.wheels[1].direction = direction;
		vehicle.wheels[1].axis = axis;
		vehicle.wheels[1].suspensionRestLength = suspensionRestLength;
		vehicle.wheels[1].radius = wheel_radius;
		vehicle.wheels[1].width = wheel_width;
		vehicle.wheels[1].front = true;
		vehicle.wheels[1].drive = true;
		vehicle.wheels[1].brake = false;
		vehicle.wheels[1].steering = true;
	}
										   break;
	case VEHICLE_TRUCK: {
		vehicle.chassis_size.Set(2, 2, 10);
		vehicle.chassis_offset.Set(0, 1.5, 0);
		vehicle.mass = 2000.0f;
		vehicle.suspensionStiffness = 5.88f;
		vehicle.suspensionCompression = 0.83f;
		vehicle.suspensionDamping = 0.88f;
		vehicle.maxSuspensionTravelCm = 100.0f;
		vehicle.frictionSlip = 50.5;
		vehicle.maxSuspensionForce = 6000.0f;

		float connection_height = 1.2f;
		float wheel_radius = 1.2f;
		float wheel_width = 0.8f;
		float suspensionRestLength = 1.7f;

		// Don't change anything below this line ------------------

		float half_width = vehicle.chassis_size.x*0.5f;
		float half_length = vehicle.chassis_size.z*0.5f;

		vec3 direction(0, -1, 0);
		vec3 axis(-1, 0, 0);

		vehicle.num_wheels = 6;
		vehicle.wheels = new Wheel[6];

		// FRONT-LEFT ------------------------
		vehicle.wheels[0].connection.Set(half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
		vehicle.wheels[0].direction = direction;
		vehicle.wheels[0].axis = axis;
		vehicle.wheels[0].suspensionRestLength = suspensionRestLength;
		vehicle.wheels[0].radius = wheel_radius;
		vehicle.wheels[0].width = wheel_width;
		vehicle.wheels[0].front = true;
		vehicle.wheels[0].drive = true;
		vehicle.wheels[0].brake = false;
		vehicle.wheels[0].steering = true;

		// FRONT-RIGHT ------------------------
		vehicle.wheels[1].connection.Set(-half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
		vehicle.wheels[1].direction = direction;
		vehicle.wheels[1].axis = axis;
		vehicle.wheels[1].suspensionRestLength = suspensionRestLength;
		vehicle.wheels[1].radius = wheel_radius;
		vehicle.wheels[1].width = wheel_width;
		vehicle.wheels[1].front = true;
		vehicle.wheels[1].drive = true;
		vehicle.wheels[1].brake = false;
		vehicle.wheels[1].steering = true;

		// REAR-LEFT ------------------------
		vehicle.wheels[2].connection.Set(half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
		vehicle.wheels[2].direction = direction;
		vehicle.wheels[2].axis = axis;
		vehicle.wheels[2].suspensionRestLength = suspensionRestLength;
		vehicle.wheels[2].radius = wheel_radius;
		vehicle.wheels[2].width = wheel_width;
		vehicle.wheels[2].front = false;
		vehicle.wheels[2].drive = false;
		vehicle.wheels[2].brake = false;
		vehicle.wheels[2].steering = false;

		// REAR-RIGHT ------------------------
		vehicle.wheels[3].connection.Set(-half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
		vehicle.wheels[3].direction = direction;
		vehicle.wheels[3].axis = axis;
		vehicle.wheels[3].suspensionRestLength = suspensionRestLength;
		vehicle.wheels[3].radius = wheel_radius;
		vehicle.wheels[3].width = wheel_width;
		vehicle.wheels[3].front = false;
		vehicle.wheels[3].drive = false;
		vehicle.wheels[3].brake = false;
		vehicle.wheels[3].steering = false;

		vehicle.wheels[4].connection.Set(half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius * 3);
		vehicle.wheels[4].direction = direction;
		vehicle.wheels[4].axis = axis;
		vehicle.wheels[4].suspensionRestLength = suspensionRestLength;
		vehicle.wheels[4].radius = wheel_radius;
		vehicle.wheels[4].width = wheel_width;
		vehicle.wheels[4].front = false;
		vehicle.wheels[4].drive = false;
		vehicle.wheels[4].brake = true;
		vehicle.wheels[4].steering = false;

		vehicle.wheels[5].connection.Set(-half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius * 3);
		vehicle.wheels[5].direction = direction;
		vehicle.wheels[5].axis = axis;
		vehicle.wheels[5].suspensionRestLength = suspensionRestLength;
		vehicle.wheels[5].radius = wheel_radius;
		vehicle.wheels[5].width = wheel_width;
		vehicle.wheels[5].front = false;
		vehicle.wheels[5].drive = false;
		vehicle.wheels[5].brake = true;
		vehicle.wheels[5].steering = false;
	}
		break;

	case VEHICLE_TRIPODE: {
		{
			vehicle.chassis_size.Set(2, 2, 4);
			vehicle.chassis_offset.Set(0, 1.5, 0);
			vehicle.mass = 500.0f;
			vehicle.suspensionStiffness = 15.88f;
			vehicle.suspensionCompression = 0.83f;
			vehicle.suspensionDamping = 0.88f;
			vehicle.maxSuspensionTravelCm = 1000.0f;
			vehicle.frictionSlip = 50.5;
			vehicle.maxSuspensionForce = 6000.0f;

			float connection_height = 1.2f;
			float wheel_radius = 0.6f;
			float wheel_width = 0.5f;
			float suspensionRestLength = 1.2f;

			float half_width = vehicle.chassis_size.x*0.5f;
			float half_length = vehicle.chassis_size.z*0.5f;

			vec3 direction(0, -1, 0);
			vec3 axis(-1, 0, 0);

			vehicle.num_wheels = 3;
			vehicle.wheels = new Wheel[3];

			vehicle.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
			vehicle.wheels[0].direction = direction;
			vehicle.wheels[0].axis = axis;
			vehicle.wheels[0].suspensionRestLength = suspensionRestLength;
			vehicle.wheels[0].radius = wheel_radius;
			vehicle.wheels[0].width = wheel_width;
			vehicle.wheels[0].front = true;
			vehicle.wheels[0].drive = true;
			vehicle.wheels[0].brake = false;
			vehicle.wheels[0].steering = true;

			vehicle.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
			vehicle.wheels[1].direction = direction;
			vehicle.wheels[1].axis = axis;
			vehicle.wheels[1].suspensionRestLength = suspensionRestLength;
			vehicle.wheels[1].radius = wheel_radius;
			vehicle.wheels[1].width = wheel_width;
			vehicle.wheels[1].front = true;
			vehicle.wheels[1].drive = true;
			vehicle.wheels[1].brake = false;
			vehicle.wheels[1].steering = true;

			vehicle.wheels[2].connection.Set(0, connection_height, -half_length + wheel_radius);
			vehicle.wheels[2].direction = direction;
			vehicle.wheels[2].axis = axis;
			vehicle.wheels[2].suspensionRestLength = suspensionRestLength;
			vehicle.wheels[2].radius = wheel_radius;
			vehicle.wheels[2].width = wheel_width;
			vehicle.wheels[2].front = false;
			vehicle.wheels[2].drive = false;
			vehicle.wheels[2].brake = true;
			vehicle.wheels[2].steering = false;
		}
	}
		break;
		case VEHICLE_STRAMBOTIC: {
		{
			vehicle.chassis_size.Set(2, 2, 7);
			vehicle.chassis_offset.Set(0, 1.5, 0);
			vehicle.mass = 500.0f;
			vehicle.suspensionStiffness = 15.88f;
			vehicle.suspensionCompression = 0.83f;
			vehicle.suspensionDamping = 0.88f;
			vehicle.maxSuspensionTravelCm = 1000.0f;
			vehicle.frictionSlip = 50.5;
			vehicle.maxSuspensionForce = 6000.0f;

			float connection_height = 1.2f;
			float wheel_radius = 0.6f;
			float wheel_width = 0.5f;
			float suspensionRestLength = 1.2f;

			float half_width = vehicle.chassis_size.x*0.5f;
			float half_length = vehicle.chassis_size.z*0.5f;

			vec3 direction(0, -1, 0);
			vec3 axis(-1, 0, 0);

			vehicle.num_wheels = 3;
			vehicle.wheels = new Wheel[3];

			vehicle.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
			vehicle.wheels[0].direction = direction;
			vehicle.wheels[0].axis = axis;
			vehicle.wheels[0].suspensionRestLength = suspensionRestLength;
			vehicle.wheels[0].radius = wheel_radius * 2;
			vehicle.wheels[0].width = wheel_width * 2;
			vehicle.wheels[0].front = true;
			vehicle.wheels[0].drive = true;
			vehicle.wheels[0].brake = false;
			vehicle.wheels[0].steering = true;

			vehicle.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
			vehicle.wheels[1].direction = direction;
			vehicle.wheels[1].axis = axis;
			vehicle.wheels[1].suspensionRestLength = suspensionRestLength;
			vehicle.wheels[1].radius = wheel_radius * 2;
			vehicle.wheels[1].width = wheel_width * 2;
			vehicle.wheels[1].front = true;
			vehicle.wheels[1].drive = true;
			vehicle.wheels[1].brake = false;
			vehicle.wheels[1].steering = true;

			vehicle.wheels[2].connection.Set(0, connection_height, +half_length - wheel_radius);
			vehicle.wheels[2].direction = direction;
			vehicle.wheels[2].axis = axis;
			vehicle.wheels[2].suspensionRestLength = suspensionRestLength;
			vehicle.wheels[2].radius = wheel_radius;
			vehicle.wheels[2].width = wheel_width;
			vehicle.wheels[2].front = false;
			vehicle.wheels[2].drive = false;
			vehicle.wheels[2].brake = true;
			vehicle.wheels[2].steering = false;
		}
	}
		break;
	
	}
	return vehicle;

}