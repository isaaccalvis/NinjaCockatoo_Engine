#include "Globals.h"
#include "Application.h"
#include "SceneImporter.h"

#include "Mesh.h"
#include "C_Mesh.h"
#include "C_Material.h"

#include "physfs/include/physfs.h"

#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/cimport.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Assimp/libx86/zlibstatic.lib")

void myCallback(const char* msg, char* userData)
{
	LOG_CONSOLE("%s", msg);
}

SceneImporter::SceneImporter()
{
	bool ret = true;
	struct aiLogStream stream;
	stream.callback = myCallback;
	aiAttachLogStream(&stream);
}

SceneImporter::~SceneImporter()
{
	aiDetachAllLogStreams();
}

void SceneImporter::Import(const char* path, const uuid_unit uuid, const ImporterSettings* settings)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_GenUVCoords | aiProcess_FlipUVs);
	if (!scene)
	{
		LOG_CONSOLE_IDE("Mesh can not be loaded");
		return;
	}

	SceneImporterSettings* sceneSettings = (SceneImporterSettings*)settings;
	GameObject* base_go = IterateSceneLoading(scene, scene->mRootNode, App->scene->root, sceneSettings->originalPath.c_str());
	App->fs->OnSaveScene(base_go, std::to_string(uuid), "Library/Meshes/");
	App->scene->MakeCameraLookThisGOSelected();
}

GameObject* SceneImporter::IterateSceneLoading(const aiScene* scene, const aiNode* node, GameObject* parent, const char* originalPath)
{
	// Create GameObject
	GameObject* go = App->scene->CreateGameObject(node->mName.C_Str(), parent);
	aiVector3D position;
	aiVector3D scale;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scale, rotation, position);
	go->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->position = {position.x, position.y, position.z};
	go->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->rotation = { rotation.x, rotation.y, rotation.z, rotation.w };
	go->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->scale = { scale.x, scale.y, scale.z };
	go->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrix();
	go->GetComponent(COMPONENT_TRANSFORM)->GetComponentAsTransform()->UpdateGlobalMatrixOfChilds();
	go->UpdateAABB();

	// Create & Load Mesh
	if (node->mMeshes != nullptr)
	{
		Mesh* mesh = new Mesh(scene, node);
		uuid_unit meshUUID = App->input->GenerateUUID();
		App->resources->AddResourceMesh(mesh, meshUUID);
		App->resources->GetResourceMesh(meshUUID)->name = node->mName.C_Str();
		App->fs->CreateOwnMesh(mesh, meshUUID);
		Component* compMesh = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
		compMesh->GetComponentAsMesh()->mesh_resources_uuid = meshUUID;
		Component* comTexture = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MATERIAL, "Material");
		compMesh->GetComponentAsMesh()->SetMesh(mesh);

		aiString str;
		scene->mMaterials[scene->mMeshes[(*node->mMeshes)]->mMaterialIndex]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &str);
		std::string tmp_texture_path(originalPath);
		if (sizeof(originalPath) <= 4) // Thats to check if originalPath exist
		{
			tmp_texture_path = App->fs->resources_directory + "Assets//";
		}
		tmp_texture_path.append(str.C_Str());
		// Load texture
		uuid_unit textureUUID = App->fs->DistributeObjectToLoad(tmp_texture_path.c_str());
		//App->fs->materialImporter->Import(tmp_texture_path.c_str(), App->input->GenerateUUID());
		// Search it
		std::string texture_name_and_extension(tmp_texture_path);
		texture_name_and_extension = texture_name_and_extension.substr(texture_name_and_extension.find_last_of(92) + 1);
		if (App->resources->GetResourceMaterial(textureUUID) != nullptr)
			comTexture->GetComponentAsMaterial()->SetTexture(App->resources->GetResourceMaterial(textureUUID)->texture);
		str.Clear();
	}

	// Iterate Childs
	for (int i = 0; i < node->mNumChildren; i++)
	{
		IterateSceneLoading(scene, node->mChildren[i], go, originalPath);
	}
	return go;
}

Mesh* SceneImporter::LoadMesh(const char* exportedFile)
{
	Mesh* mesh = new Mesh();
	mesh->type = MESH_TYPE::CUSTOM_MESH;

	PHYSFS_File* file = PHYSFS_openRead(exportedFile);
	if (file == nullptr)
		return nullptr;
	unsigned int size = PHYSFS_fileLength(file);
	char* data2 = new char[size];

	char* data = new char[size];
	PHYSFS_readBytes(file, data, size);
	PHYSFS_close(file);

	char* cursor = data;

	// Get Header
	unsigned int ranged[4];
	unsigned int bytes = sizeof(ranged);
	memcpy(ranged, cursor, bytes);
	mesh->SetVerticesSize(ranged[0]);
	mesh->SetIndicesSize(ranged[1]);
	mesh->SetTextureCoorSize(ranged[2]);
	mesh->SetNormalsSize(ranged[3]);

	// Get Vertices
	cursor += bytes;
	bytes = sizeof(GLfloat) * mesh->GetVerticesSize() * 3;
	mesh->verticesArray = new GLfloat[mesh->GetVerticesSize() * 3];
	memcpy(mesh->verticesArray, cursor, bytes);

	// Get Indices
	cursor += bytes;
	bytes = sizeof(unsigned int) * mesh->GetIndicesSize() * 3;
	mesh->indicesArray = new unsigned int[mesh->GetIndicesSize() * 3];
	memcpy(mesh->indicesArray, cursor, bytes);

	// TODO: mmgr give (i think) problems with that

	// Get Texture Coords
	cursor += bytes;
	bytes = sizeof(GLfloat) * mesh->GetTextureCoorSize() * 2;
	mesh->textureCoords = new GLfloat[mesh->GetTextureCoorSize() * 2];
	memcpy(mesh->textureCoords, cursor, bytes);

	// Get Normals
	cursor += bytes;
	mesh->normals = new DebugArrow[mesh->GetNormalsSize()];
	bytes = sizeof(GLfloat) * 3 * 2;
	for (int i = 0; i < mesh->GetNormalsSize(); i++)
	{
		mesh->normals[i].verticesArray = new GLfloat[6];
		memcpy(mesh->normals[i].verticesArray , cursor, bytes);
		mesh->normals[i].GenerateBuffers();
		cursor += bytes;
	}

	mesh->GenerateIndicesBuffer();
	mesh->GenerateVerticesBuffer();
	mesh->GenerateTextureCoorBuffer();

	delete[] data;

	return mesh;
}