#include "Application.h"
#include "ModuleScene.h"

#include "C_Mesh.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleScene";
}

ModuleScene::~ModuleScene()
{
	CleanUp();
}

bool ModuleScene::Start()
{
	root = new GameObject("root", nullptr);
	quadTree = new QuadTree_d();
	quadTree->Create(math::AABB(math::float3(-10, -10, -10), math::float3(10, 10, 10)), quadTree);
	return true;
}

update_status ModuleScene::Update(float dt)
{
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (App->camera->isCameraEditor)
			if (!ImGuizmo::IsOver())
				MousePicking(App->input->GetMouseX(), App->input->GetMouseY());
	}
	return update_status::UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	// Delete All GameObjects
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects.erase(gameObjects.begin() + i);
	}
	gameObjects.clear();

	// Delete root
	if (root != nullptr)
		delete root;

	return true;
}

void ModuleScene::DeleteGameObject(GameObject* go)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i] == go)
		{
			delete gameObjects[i];
			gameObjects.erase(gameObjects.begin() + i);
		}
	}
}

GameObject* ModuleScene::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* go = new GameObject(name, parent);
	if (go != nullptr)
	{
		gameObjects.push_back(go);
	}
	goSelected = go;
	
	return go;
}

GameObject* ModuleScene::SearchGameObject(std::string name)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->GetName() == name.c_str())
		{
			return gameObjects[i];
		}
	}
	return nullptr;
}

void ModuleScene::MakeCameraLookThisGOSelected()
{
	// Set camera pos
	if (goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MESH) != nullptr)
	{
		App->camera->camera.frustum.pos.y = goSelected->boundingBox.CenterPoint().y + goSelected->boundingBox.Diagonal().y * 2;
		App->camera->camera.frustum.pos.x = goSelected->boundingBox.CenterPoint().x + goSelected->boundingBox.Diagonal().x * 2;
		App->camera->camera.frustum.pos.z = goSelected->boundingBox.CenterPoint().z + goSelected->boundingBox.Diagonal().z * 2;
	}
	else
	{
		App->camera->camera.frustum.pos.x = goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position.x + 1;
		App->camera->camera.frustum.pos.y = goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position.y + 1;
		App->camera->camera.frustum.pos.z = goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position.z + 1;
	}
	// Look
	App->camera->LookAt(goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position);

}

void ModuleScene::MousePicking(int coor_x, int coor_y)
{
	int w_width;
	int w_heigh;
	int w_size;
	App->window->GetWindowSize(w_width, w_heigh, w_size);

	float x = -(1.0f - (float(coor_x) * 2.0f) / w_width);
	float y = 1.0f - (float(coor_y) * 2.0f) / w_heigh;

	math::LineSegment line = App->camera->camera.frustum.UnProjectLineSegment(x, y);

	GameObject* obj = nullptr;
	std::vector<GameObject*> selectedGO;

	if (App->scene->quadTree->useQuadTree)
	{
		std::list<GameObject*> selGo;
		if (App->scene->quadTree != nullptr)
		{
			App->scene->quadTree->Interesct(line, selGo);
		}
		selectedGO.resize(selGo.size());
		unsigned int counter = 0;
		for (std::list<GameObject*>::iterator it = selGo.begin(); it != selGo.end(); it++)
		{
			selectedGO[counter] = (*it);
			counter++;
		}
		for (int i = 0; i < selectedGO.size(); i++)
		{
			if (!line.Intersects(selectedGO[i]->boundingBox))
			{
				selectedGO.erase(selectedGO.begin() + i);
			}
		}
		for (int i = 0; i < App->scene->gameObjects.size(); i++)
		{
			if (!App->scene->gameObjects[i]->GetIsStatic())
			{
				if (line.Intersects(App->scene->gameObjects[i]->boundingBox))
				{
					obj = App->scene->gameObjects[i];
					selectedGO.push_back(obj);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < App->scene->gameObjects.size(); i++)
		{
			if (line.Intersects(App->scene->gameObjects[i]->boundingBox))
			{
				obj = App->scene->gameObjects[i];
				selectedGO.push_back(obj);
			}
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
				concreteIndice = tmp_selected_mesh->indicesArray[a];
				a++;
				tmp_triangle.a = tmp_selected_mesh->vectorVertex[concreteIndice];
				concreteIndice = tmp_selected_mesh->indicesArray[a];
				a++;
				tmp_triangle.b = tmp_selected_mesh->vectorVertex[concreteIndice];
				concreteIndice = tmp_selected_mesh->indicesArray[a];
				a++;
				tmp_triangle.c = tmp_selected_mesh->vectorVertex[concreteIndice];

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