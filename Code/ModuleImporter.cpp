#include "Application.h"

#include "ModuleImporter.h"
#include "Console.h"

#include "GameObject.h"
#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"

#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Assimp/libx86/zlibstatic.lib")

#pragma comment (lib, "DevIL/lib/DevIL.lib")
#pragma comment (lib, "DevIL/lib/ILU.lib")
#pragma comment (lib, "DevIL/lib/ILUT.lib")

void myCallback(const char* msg, char* userData)
{
	printf_s("%s", msg);
	LOG_CONSOLE("%s", msg);
}

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleImporter";
}

bool ModuleImporter::Start()
{
	bool ret = true;
	// Assimp CallBack
	struct aiLogStream stream;
	stream.callback = myCallback;
	aiAttachLogStream(&stream);

	// Init Devil
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
		ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) {
		printf("DevIL version is different...exiting!\n");
		ret = false;
	}
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilutEnable(ILUT_OPENGL_CONV);

	return ret;
}

bool ModuleImporter::CleanUp()
{
	return true;
}

bool ModuleImporter::Save(JSON_Object* root_object)
{
	json_object_set_string(root_object, "resources_directory", resources_directory.c_str());
	return true;
}

bool ModuleImporter::Load(JSON_Object* root_object)
{
	resources_directory = json_object_get_string(root_object, "resources_directory");
	return true;
}

void ModuleImporter::DistributeObjectToLoad(const char* path)
{
	std::string direction_without_name(path);
	direction_without_name = direction_without_name.substr(0, direction_without_name.find_last_of(92) + 1);
	std::string name_and_extension(path);
	name_and_extension = name_and_extension.substr(name_and_extension.find_last_of(92) + 1);
	std::string extension = name_and_extension;
	extension = extension.substr(extension.find_last_of('.') + 1);
	std::string toSavePath = resources_directory + name_and_extension;

	CopyFile(path, toSavePath.c_str(), true);

	if (extension == "fbx")
	{
		LoadScene(toSavePath.c_str(), direction_without_name.c_str());
	}
	else if (extension == "dds" || extension == "png")
	{
		LoadTexture(toSavePath.c_str());
	}
}

void ModuleImporter::LoadScene(const char* path, const char* originalPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	if (!scene)
	{
		LOG_CONSOLE_IDE("Mesh can not be loaded");
		return;
	}

	IterateSceneLoading(scene, scene->mRootNode, App->scene->root, originalPath);

	App->scene->MakeCameraLookThisGOSelected();
}

void ModuleImporter::IterateSceneLoading(const aiScene* scene, const aiNode* node, GameObject* parent, const char* originalPath)
{
	// Create GameObject
	GameObject* go = App->scene->CreateGameObject(node->mName.C_Str(), parent);


	// Create & Load Mesh
	if (node->mMeshes != nullptr)
	{
		Mesh* mesh = new Mesh(scene, node);
		App->renderer3D->AddMesh(mesh);
		Component* compMesh = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
		Component* comTexture = go->CreateComponent(COMPONENT_TYPE::COMPONENT_MATERIAL, "Material");
		compMesh->GetComponentAsMesh()->mesh = mesh;

		aiString str;
		scene->mMaterials[scene->mMeshes[(*node->mMeshes)]->mMaterialIndex]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &str);
		std::string tmp_texture_path(originalPath);
		if (sizeof(originalPath) <= 4) // Thats to check if originalPath exist
		{
			tmp_texture_path = "Resources/";
		}
		tmp_texture_path.append(str.C_Str());
		comTexture->GetComponentAsMaterial()->texture = App->importer->LoadTexture(tmp_texture_path.c_str());
		str.Clear();
	}

	// Iterate Childs
	for (int i = 0; i < node->mNumChildren; i++)
	{
		IterateSceneLoading(scene, node->mChildren[i], go, originalPath);
	}

	// Look if there are more meshes at same Node
	for (int i = 1; i < node->mNumMeshes; i++)
	{
		// Create GameObject
		GameObject* auxGo = App->scene->CreateGameObject(node->mName.C_Str(), go);

		// Create & Load Mesh & Load Texture
		if (node->mMeshes != nullptr)
		{
			Mesh* mesh = new Mesh(scene, node, i);
			App->renderer3D->AddMesh(mesh);
			Component* compMesh = auxGo->CreateComponent(COMPONENT_TYPE::COMPONENT_MESH, "Mesh");
			compMesh->GetComponentAsMesh()->mesh = mesh;
			Component* comTexture = auxGo->CreateComponent(COMPONENT_TYPE::COMPONENT_MATERIAL, "Material");

			aiString str;
			scene->mMaterials[scene->mMeshes[(*node->mMeshes)]->mMaterialIndex]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &str);
			std::string tmp_texture_path(originalPath);
			tmp_texture_path.append(str.C_Str());
			comTexture->GetComponentAsMaterial()->texture = App->importer->LoadTexture(tmp_texture_path.c_str());
			str.Clear();
		}
	}

}

Texture* ModuleImporter::LoadTexture(const char* path)
{
	Texture* texture = new Texture();

	ILuint ImageName;
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);

	uint textureNum = 0u;

	// Loading image
	if (ilLoadImage(path))
	{
		ILinfo imageInfo;
		iluGetImageInfo(&imageInfo);
		texture->SetWidth(imageInfo.Width);
		texture->SetHeight(imageInfo.Height);
		if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &textureNum);
			glBindTexture(GL_TEXTURE_2D, textureNum);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_WIDTH),
				0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			App->console->AddLog("ilConvertImage failed :: %s", iluErrorString(ilGetError()));
	}
	else
		App->console->AddLog("ilLoadImage failed :: %s", iluErrorString(ilGetError()));

	ilDeleteImages(1, &ImageName);

	texture->SetBufferPos(textureNum);

	App->textures->AddTexture(texture);

	// Add loaded texture to selected mesh
	if (App->scene->goSelected != nullptr)
	{
		if (App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL) != nullptr)
		{
			App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->texture = texture;
		}
		else
		{
			App->scene->goSelected->CreateComponent(COMPONENT_TYPE::COMPONENT_MATERIAL, "Material");
			App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->texture = texture;
		}
	}

	return texture;
}

const std::string ModuleImporter::GetResourcesDirectory() const
{
	return resources_directory;
}

void ModuleImporter::SetResourcesDirectory(const std::string str)
{
	resources_directory = str;
}