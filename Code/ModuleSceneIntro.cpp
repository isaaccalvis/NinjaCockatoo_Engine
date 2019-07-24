#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"

#include "ModulePlayer.h"

#define SIZE 3
#define SPACE_PART_CIRCUIT 4
#define RENDER_DISTANCE 30

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	Plane p2(0,1,0,0);
	p = p2;
	LOG("Loading Intro assets");
	bool ret = true;
	createMap1();
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	mapXML->reset();
	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	// AIXÒ ES PER PINTAR LA MALLA, ES CAPA AMB EL BOTO F1 I UNA VARIABLE BOOL A PLAYER !!!
	p2List_item<Primitive*>* rec = primitiveListMesh.getFirst();
	while (rec != nullptr) {
		int xx, yy;
		rec->data->getXY(xx, yy);
		int xFinal = xx - (int)App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX();
		if (xFinal < 0)
			xFinal *= -1;
		int yFinal = yy - (int)App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ();// Sha de pillar la Z perque Y es alçada
		if (yFinal < 0)
			yFinal *= -1;
		if (xFinal < RENDER_DISTANCE && yFinal < RENDER_DISTANCE)
			if (App->player->drawMeshes)
				rec->data->Render();
		rec = rec->next;
	}
	p.Render();

	return UPDATE_CONTINUE;
}

int ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {
	int cont = 0;
	if (body1 == App->player->vehicle) {
		if (body2 == sensor[0]) {
			cont++;
			if (sensorState[0] == SENSOR_EXIT) {
				sensorState[0] = SENSOR_ENTER;
				if (sensorActive[0] == false)
					sensorActive[0] = true;
				else
					sensorActive[0] = false;
				sensorState[1] = SENSOR_EXIT;
				sensorState[2] = SENSOR_EXIT;
				sensorState[3] = SENSOR_EXIT;
			}
			comprobarMetaDeSensors(0);
		}
		else if (body2 == sensor[1]) {
			cont++;
			if (sensorState[1] == SENSOR_EXIT) {
				sensorState[1] = SENSOR_ENTER;
				if (sensorActive[1] == false)
					sensorActive[1] = true;
				else
					sensorActive[1] = false;
				sensorState[0] = SENSOR_EXIT;
				sensorState[2] = SENSOR_EXIT;
				sensorState[3] = SENSOR_EXIT;
			}
			comprobarMetaDeSensors(1);
		}
		else if (body2 == sensor[2]) {
			cont++;
			if (sensorState[2] == SENSOR_EXIT) {
				sensorState[2] = SENSOR_ENTER;
				if (sensorActive[2] == false)
					sensorActive[2] = true;
				else
					sensorActive[2] = false;
				sensorState[0] = SENSOR_EXIT;
				sensorState[1] = SENSOR_EXIT;
				sensorState[3] = SENSOR_EXIT;
				comprobarMetaDeSensors(2);
			}
		}
		else if (body2 == sensor[3]) {
			cont++;
			if (sensorState[3] == SENSOR_EXIT) {
				sensorState[3] = SENSOR_ENTER;
				if (sensorActive[3] == false)
					sensorActive[3] = true;
				else
					sensorActive[3] = false;
				sensorState[0] = SENSOR_EXIT;
				sensorState[1] = SENSOR_EXIT;
				sensorState[2] = SENSOR_EXIT;
				comprobarMetaDeSensors(3);
			}
		}
	}
	else if (body2 == App->player->vehicle) {
		if (body1 == sensor[0]) {
			cont++;
			if (sensorState[0] == SENSOR_EXIT) {
				sensorState[0] = SENSOR_ENTER;
				if (sensorActive[0] == false)
					sensorActive[0] = true;
				else
					sensorActive[0] = false;
				sensorState[1] = SENSOR_EXIT;
				sensorState[2] = SENSOR_EXIT;
				sensorState[3] = SENSOR_EXIT;
				comprobarMetaDeSensors(0);
			}
		}
		else if (body1 == sensor[1]) {
			cont++;
			if (sensorState[1] == SENSOR_EXIT) {
				sensorState[1] = SENSOR_ENTER;
				if (sensorActive[1] == false)
					sensorActive[1] = true;
				else
					sensorActive[1] = false;
				sensorState[0] = SENSOR_EXIT;
				sensorState[2] = SENSOR_EXIT;
				sensorState[3] = SENSOR_EXIT;
				comprobarMetaDeSensors(1);
			}
		}
		else if (body1 == sensor[2]) {
			cont++;
			if (sensorState[2] == SENSOR_EXIT) {
				sensorState[2] = SENSOR_ENTER;
				if (sensorActive[2] == false)
					sensorActive[2] = true;
				else
					sensorActive[2] = false;
				sensorState[0] = SENSOR_EXIT;
				sensorState[1] = SENSOR_EXIT;
				sensorState[3] = SENSOR_EXIT;
				comprobarMetaDeSensors(2);
			}
		}
		else if (body1 == sensor[3]) {
			cont++;
			if (sensorState[3] == SENSOR_EXIT) {
				sensorState[3] = SENSOR_ENTER;
				if (sensorActive[3] == false)
					sensorActive[3] = true;
				else
					sensorActive[3] = false;
				sensorState[0] = SENSOR_EXIT;
				sensorState[1] = SENSOR_EXIT;
				sensorState[2] = SENSOR_EXIT;
				comprobarMetaDeSensors(3);
			}
		}
	}

	return cont;
}

void ModuleSceneIntro::createMap1() {
	netejarSensor(true);
	//if (mapXML != nullptr)
	//	mapXML->reset();
	mapXML = new pugi::xml_document;
	mapXML->load_file("mapa.tmx");
	pugi::xml_node layer = mapXML->child("map").child("layer");
	std::string nullLayerString = "";
	std::string elementsName = "elements";
	std::string logicName = "logic";
	while (layer.attribute("name").as_string() != nullLayerString) {
		pugi::xml_node layer_data = layer.child("data");
		pugi::xml_node tile = layer_data.child("tile");

		for (int y = 0; y < mapXML->child("map").attribute("height").as_uint(); ++y)
		{
			for (int x = 0; x < mapXML->child("map").attribute("width").as_int(); ++x)
			{
				switch (tile.attribute("gid").as_int() - 1) {
				case 0:
					break;
				case 1: {
					if (layer.attribute("name").as_string() == logicName) {
						Cube cube(SIZE, SIZE, SIZE);
						Cube cube2(SIZE, SIZE, SIZE);
						Cube cube3(SIZE, SIZE, SIZE);
						cube.SetPos(x * SPACE_PART_CIRCUIT * SIZE, SIZE * 2, y * SPACE_PART_CIRCUIT * SIZE);
						cube2.SetPos(x * SPACE_PART_CIRCUIT * SIZE, 0, y * SPACE_PART_CIRCUIT * SIZE * 1.25f);
						cube3.SetPos(x * SPACE_PART_CIRCUIT * SIZE, SIZE * SPACE_PART_CIRCUIT, y * SPACE_PART_CIRCUIT * SIZE);
						PhysBody3D *c1 = App->physics->AddBody(cube, 0);
						PhysBody3D *c2 = App->physics->AddBody(cube2, 5, true);
						PhysBody3D *c3 = App->physics->AddBody(cube3, 0, true);

						App->physics->AddConstraintHinge(*c1, *c2, vec3(0, 0, 0), vec3(0, -5, -5), vec3(1, 0, 0), vec3(1, 0, 0), false);
					}
				}
						break;
				case 2: {
					if (layer.attribute("name").as_string() == logicName) {
						Cube cube(SIZE, SIZE, SIZE);
						Cube cube2(SIZE, SIZE, SIZE);
						Cube cube3(SIZE, SIZE, SIZE);
						cube.SetPos(x * SPACE_PART_CIRCUIT * SIZE, SIZE * 2, y * SPACE_PART_CIRCUIT * SIZE);
						cube2.SetPos(x * SPACE_PART_CIRCUIT * SIZE, 0, y * SPACE_PART_CIRCUIT * SIZE * 1.25f);
						cube3.SetPos(x * SPACE_PART_CIRCUIT * SIZE, SIZE * SPACE_PART_CIRCUIT, y * SPACE_PART_CIRCUIT * SIZE);
						PhysBody3D *c1 = App->physics->AddBody(cube, 0);
						PhysBody3D *c2 = App->physics->AddBody(cube2, 5, true);
						PhysBody3D *c3 = App->physics->AddBody(cube3, 0, true);

						App->physics->AddConstraintHinge(*c1, *c2, vec3(0, 0, 0), vec3(0, 5, 5), vec3(1, 0, 0), vec3(1, 0, 0), false);
					}
				}
						break;
				case 3: {
					if (layer.attribute("name").as_string() == logicName) {
						Cube cube(SIZE, SIZE, SIZE);
						Cube cube2(SIZE, SIZE, SIZE);
						cube.SetPos(x * SPACE_PART_CIRCUIT * SIZE, SIZE * 2, y * SPACE_PART_CIRCUIT * SIZE);
						cube2.SetPos(x * SPACE_PART_CIRCUIT * SIZE, 0, y * SPACE_PART_CIRCUIT * SIZE * 1.25f);
						PhysBody3D *c1 = App->physics->AddBody(cube, 0, true);
						PhysBody3D *c2 = App->physics->AddBody(cube2, 1, true);

						App->physics->AddConstraintP2P(*c1, *c2, vec3(2, 2, 2), vec3(0, 6, 0));
					}
				}
						break;
				case 4: {
					if (layer.attribute("name").as_string() == logicName) {
						Cube seCub(SIZE * 2, SIZE, SIZE * 2);
						seCub.SetPos(x * SPACE_PART_CIRCUIT * SIZE, SIZE / 2, y * SPACE_PART_CIRCUIT * SIZE);
						// Aixo son els sensors
						if (sensor[0] == nullptr) {
							sensor[0] = App->physics->AddBody(seCub, 0);
							sensor[0]->SetAsSensor(true);
							sensor[0]->collision_listeners.add(this);
						}
						else if (sensor[1] == nullptr) {
							sensor[1] = App->physics->AddBody(seCub, 0);
							sensor[1]->SetAsSensor(true);
							sensor[1]->collision_listeners.add(this);
						}
						else if (sensor[2] == nullptr) {
							sensor[2] = App->physics->AddBody(seCub, 0);
							sensor[2]->SetAsSensor(true);
							sensor[2]->collision_listeners.add(this);
						}
						else if (sensor[3] == nullptr) {
							sensor[3] = App->physics->AddBody(seCub, 0);
							sensor[3]->SetAsSensor(true);
							sensor[3]->collision_listeners.add(this);
						}
						else {
							LOG("You have added a sensor that can't be placed ! (sensor max = 4)");
							netejarSensor();
							system("mapa.tmx");
							createMap1();
							return;
						}
					}
				}
						break;
				case 5:
					if (layer.attribute("name").as_string() == logicName)
						addPrimitiveToMap(CUBE, x * SIZE* SPACE_PART_CIRCUIT, 0, y * SIZE* SPACE_PART_CIRCUIT, SIZE, SIZE, SIZE);
					break;
				case 6:
					if (layer.attribute("name").as_string() == logicName)
						addPrimitiveToMap(SPHERE, x * SIZE* SPACE_PART_CIRCUIT, 0, y * SIZE* SPACE_PART_CIRCUIT, SIZE, SIZE, SIZE);
					break;
				case 7:
					if (layer.attribute("name").as_string() == logicName)
						addMeshToMap(Primitive_Custom, "meshes/arbre.obj", x * SPACE_PART_CIRCUIT * SIZE, SIZE * 2, y * SIZE * SPACE_PART_CIRCUIT, 90);
					break;
				case 8:
					break;
				case 9:
					if (layer.attribute("name").as_string() == elementsName)
						addPrimitiveToMap(CIRCUIT_RECTA, x * SPACE_PART_CIRCUIT * SIZE, 1, y * SPACE_PART_CIRCUIT * SIZE, SIZE, SIZE, SIZE);
					break;
				case 10:
					if (layer.attribute("name").as_string() == elementsName)
						addPrimitiveToMap(CIRCUIT_ROTA_FRONT_RIGHT, x * SPACE_PART_CIRCUIT * SIZE, 1, y * SPACE_PART_CIRCUIT * SIZE, SIZE, SIZE, SIZE);
					break;
				case 11:
					if (layer.attribute("name").as_string() == elementsName)
						addPrimitiveToMap(CIRCUIT_ROTA_FRONT_LEFT, x * SPACE_PART_CIRCUIT * SIZE, 1, y * SPACE_PART_CIRCUIT * SIZE, SIZE, SIZE, SIZE);
					break;
				case 12:
					if (layer.attribute("name").as_string() == elementsName)
						addPrimitiveToMap(CIRCUIT_ROTA_BACK_RIGHT, x * SPACE_PART_CIRCUIT * SIZE, 1, y * SPACE_PART_CIRCUIT * SIZE, SIZE, SIZE, SIZE);
					break;
				case 13:
					if (layer.attribute("name").as_string() == elementsName)
						addPrimitiveToMap(CIRCUIT_ROTA_BACK_LEFT, x * SPACE_PART_CIRCUIT * SIZE, 1, y * SPACE_PART_CIRCUIT * SIZE, SIZE, SIZE, SIZE);
					break;
				case 14:
					if (layer.attribute("name").as_string() == elementsName)
						addPrimitiveToMap(CIRCUIT_RECTA_LATERAL, x * SPACE_PART_CIRCUIT * SIZE, 1, y * SPACE_PART_CIRCUIT * SIZE, SIZE, SIZE, SIZE);
					break;
				}
				tile = tile.next_sibling("tile");
			}
		}
		layer = layer.next_sibling("layer");
	}
}

void ModuleSceneIntro::addPrimitiveToMap(primitiveTypes type, int x, int y, int z, int radOrX, int Y = 1, int Z = 1) {
	switch (type) {
	case CUBE: {
		Cube cube(radOrX, Y, Z);
		cube.SetPos(x, y, z);
		App->physics->AddBody(cube, 1,true);
		break;
	}
	case SPHERE: {
		Sphere sphere(radOrX);
		sphere.SetPos(x, y, z);
		App->physics->AddBody(sphere, 1,true);
		break;
	}
	case CIRCUIT_RECTA: {
		Cube cube(radOrX, Y , Z * SPACE_PART_CIRCUIT);
		Cube cube2(radOrX, Y , Z * SPACE_PART_CIRCUIT);
		cube.SetPos(x + SIZE * SPACE_PART_CIRCUIT/2, y, z );
		addMeshToMap(Primitive_Custom, "meshes/balla2.obj", x + SIZE * SPACE_PART_CIRCUIT / 2.3f, 0, z, 0);
		cube2.SetPos(x - SIZE * SPACE_PART_CIRCUIT/2, y, z );
		addMeshToMap(Primitive_Custom, "meshes/balla2.obj", x - SIZE * SPACE_PART_CIRCUIT / 2.3f, 0, z, 0);
		App->physics->AddBody(cube, 0);
		App->physics->AddBody(cube2, 0);
		break;
	}
	case CIRCUIT_ROTA_FRONT_RIGHT: {
		Cube cube(radOrX, Y, Z * SPACE_PART_CIRCUIT / 4 * 3);
		Cube cube2(radOrX * SPACE_PART_CIRCUIT, Y, Z);
		cube.SetPos(x - SIZE * SPACE_PART_CIRCUIT / 2, y, z);
		cube2.SetPos(x, y, z - SIZE * SPACE_PART_CIRCUIT / 2);
		addMeshToMap(Primitive_Custom, "meshes/esquina.obj", x - SIZE * SPACE_PART_CIRCUIT / 2.3f, 0, z, 0);
		App->physics->AddBody(cube, 0);
		App->physics->AddBody(cube2, 0);
	}
		break;
	case CIRCUIT_ROTA_FRONT_LEFT: {
		Cube cube(radOrX, Y, Z * SPACE_PART_CIRCUIT / 4 * 3);
		Cube cube2(radOrX * SPACE_PART_CIRCUIT, Y, Z);
		cube.SetPos(x + SIZE * SPACE_PART_CIRCUIT / 2, y, z);
		cube2.SetPos(x, y, z - SIZE * SPACE_PART_CIRCUIT / 2);
		addMeshToMap(Primitive_Custom, "meshes/esquina.obj", x , 0, z - SIZE * SPACE_PART_CIRCUIT / 2.3f, 270);
		App->physics->AddBody(cube, 0);
		App->physics->AddBody(cube2, 0);
	}
		break;
	case CIRCUIT_ROTA_BACK_RIGHT: {
		Cube cube(radOrX, Y, Z * SPACE_PART_CIRCUIT / 4 * 3);
		Cube cube2(radOrX * SPACE_PART_CIRCUIT, Y, Z);
		cube.SetPos(x - SIZE * SPACE_PART_CIRCUIT / 2, y, z);
		cube2.SetPos(x, y, z + SIZE * SPACE_PART_CIRCUIT / 2);
		addMeshToMap(Primitive_Custom, "meshes/esquina.obj", x, 0, z + SIZE * SPACE_PART_CIRCUIT / 2.3f, 90);
		App->physics->AddBody(cube, 0);
		App->physics->AddBody(cube2, 0);
	}
		break;
	case CIRCUIT_ROTA_BACK_LEFT: {
		Cube cube(radOrX, Y, Z * SPACE_PART_CIRCUIT / 4 * 3);
		Cube cube2(radOrX * SPACE_PART_CIRCUIT, Y, Z);
		cube.SetPos(x + SIZE * SPACE_PART_CIRCUIT / 2, y, z);
		cube2.SetPos(x, y, z + SIZE * SPACE_PART_CIRCUIT / 2);
		addMeshToMap(Primitive_Custom, "meshes/esquina.obj", x + SIZE * SPACE_PART_CIRCUIT / 2.3f, 0, z , 180);
		App->physics->AddBody(cube, 0);
		App->physics->AddBody(cube2, 0);
		
	}
		break;
	case CIRCUIT_RECTA_LATERAL:
		Cube cube(radOrX * SPACE_PART_CIRCUIT, Y, Z);
		Cube cube2(radOrX * SPACE_PART_CIRCUIT, Y, Z);
		cube.SetPos(x, y, z + SIZE * SPACE_PART_CIRCUIT / 2);
		addMeshToMap(Primitive_Custom, "meshes/balla2.obj", x, 0, z + SIZE * SPACE_PART_CIRCUIT / 2.3f, 90);
		cube2.SetPos(x, y, z - SIZE * SPACE_PART_CIRCUIT / 2);
		addMeshToMap(Primitive_Custom, "meshes/balla2.obj", x, 0, z - SIZE * SPACE_PART_CIRCUIT / 2.3f, 90);
		App->physics->AddBody(cube, 0);
		App->physics->AddBody(cube2, 0);
		break;
	}
}

void ModuleSceneIntro::addMeshToMap(PrimitiveTypes type, char* path, int x, int y, int z, int degToRotate = 0) {
	switch (type) {
	case	Primitive_Custom: {
		Primitive* item = new Custom(path,x,y,z,degToRotate);
		primitiveListMesh.add(item);
	}
	}
}

void ModuleSceneIntro::CleanMeshes() {
	p2List_item<Primitive*>* rec = primitiveListMesh.getFirst();
	while (rec != nullptr) {
		p2List_item<Primitive*>* aux = rec;
		rec = rec->next;
		aux->data->clearPrimitive();
		primitiveListMesh.del(aux);
	}
	primitiveListMesh.clear();
}

void ModuleSceneIntro::netejarSensor(bool netejaTotal) {
	if (netejaTotal == true) {
		sensor[0] = nullptr;
		sensor[1] = nullptr;
		sensor[2] = nullptr;
		sensor[3] = nullptr;
	}

	sensorState[0] = SENSOR_EXIT;
	sensorState[1] = SENSOR_EXIT;
	sensorState[2] = SENSOR_EXIT;
	sensorState[3] = SENSOR_EXIT;

	sensorActive[0] = false;
	sensorActive[1] = false;
	sensorActive[2] = false;
	sensorActive[3] = false;
}

void ModuleSceneIntro::comprobarMetaDeSensors(int thisSensor) {
	if (sensor[3] != nullptr) {
		if (sensorActive[0] && sensorActive[1] && sensorActive[2] && sensorActive[3]) {
			loops++;
			netejarSensor(false);
		}
	}
	else if (sensor[1] == nullptr) {
		if (sensorActive[0]) {
			loops++;
			netejarSensor(false);
		}
	}
	else if (sensor[2] == nullptr) {
		if (sensorActive[0] && sensorActive[1]) {
			loops++;
			netejarSensor(false);
		}
	}
	else if (sensor[3] == nullptr) {	// Aixo vol dir que nomes hi ha 3 sensors
		if (sensorActive[0] && sensorActive[1] && sensorActive[2]) {
			loops++;
			netejarSensor(false);
		}
	}
	if (thisSensor != -1) {
		sensorState[thisSensor] = SENSOR_ENTER;
	}
}