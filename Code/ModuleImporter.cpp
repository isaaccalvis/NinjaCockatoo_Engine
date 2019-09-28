#include "Application.h"
#include "ModuleImporter.h"
#include "Console.h"

#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/cimport.h"

#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Assimp/libx86/zlibstatic.lib")

#pragma comment (lib, "DevIL/libx86/Release/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/Release/ILU.lib")
#pragma comment (lib, "DevIL/libx86/Release/ILUT.lib")

#include "mmgr/mmgr.h"

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

update_status ModuleImporter::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
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
	std::string name_extension(path);
	name_extension = name_extension.substr(name_extension.find_last_of(92) + 1);
	std::string extension = name_extension;
	extension = extension.substr(extension.find_last_of('.') + 1);
	std::string finalPath = resources_directory + name_extension;

	CopyFile(path, finalPath.c_str(), true);

	if (extension == "fbx")
	{
		LoadMesh(finalPath.c_str());
	}
	else if (extension == "dds" || extension == "png")
	{
		LoadTexture(finalPath.c_str());
	}
}

// TODO: Carregar la mesh aqui
void ModuleImporter::LoadMesh(const char* path)
{
	App->meshes->AddCustomMesh(path);
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
		texture->width = imageInfo.Width;
		texture->heigh = imageInfo.Height;
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

	texture->BufferPos = textureNum;

	App->textures->AddTexture(texture);
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