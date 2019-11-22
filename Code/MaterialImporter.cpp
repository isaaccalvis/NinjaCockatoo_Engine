#include "Globals.h"
#include "Application.h"
#include "MaterialImporter.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"

#include "C_Material.h"

#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

#pragma comment (lib, "DevIL/lib/DevIL.lib")
#pragma comment (lib, "DevIL/lib/ILU.lib")
#pragma comment (lib, "DevIL/lib/ILUT.lib")

#include "physfs/include/physfs.h"

MaterialImporter::MaterialImporter()
{
	// Init Devil
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
		ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) {
		printf("DevIL version is different...exiting!\n");
	}
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilutEnable(ILUT_OPENGL_CONV);
}

MaterialImporter::~MaterialImporter()
{

}

void MaterialImporter::Import(const char* path, const uuid_unit uuid, const ImporterSettings* settings)
{
	Texture* texture = App->textures->SearchTexture(uuid);
	if (texture == nullptr)
	{

		std::string name_and_extension(path);
		name_and_extension = name_and_extension.substr(name_and_extension.find_last_of(92) + 1);

		texture = new Texture(name_and_extension.c_str());

		ILuint ImageName;
		ilGenImages(1, &ImageName);
		ilBindImage(ImageName);

		unsigned int textureNum = 0u;

		// Loading image
		if (ilLoadImage(path))
		{
			ILubyte *data;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
			ILuint size = ilSaveL(IL_DDS, NULL, 0);
			data = new ILubyte[size];
			if (ilSaveL(IL_DDS, data, size))
			{
				texture->textureUUID = uuid;
				PHYSFS_File* file = PHYSFS_openWrite((App->fs->resources_directory + "Library/" + "Materials/" + std::to_string(texture->textureUUID) + App->fs->texture_file_extension).c_str());
				if (file != nullptr)
					PHYSFS_writeBytes(file, data, size);
				PHYSFS_close(file);
			}

			ILinfo imageInfo;
			iluGetImageInfo(&imageInfo);
			texture->SetWidth(imageInfo.Width);
			texture->SetHeight(imageInfo.Height);
			// TODO: If extension is TGA you'll have to flip this concrete texture
			//if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			//	iluFlipImage();

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

	}
	// Add loaded texture to selected mesh
	if (App->scene->goSelected != nullptr)
	{
		if (App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL) != nullptr)
		{
			App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->SetTexture(texture);
		}
		else
		{
			App->scene->goSelected->CreateComponent(COMPONENT_TYPE::COMPONENT_MATERIAL, "Material");
			App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->SetTexture(texture);
		}
	}
}

void MaterialImporter::Load(const char* exportedFile)
{

}

Texture* MaterialImporter::LoadTexture(const char* exportedFile)
{
	std::string name_and_extension(exportedFile);
	name_and_extension = name_and_extension.substr(name_and_extension.find_last_of(92) + 1);

	Texture* texture = new Texture(name_and_extension.c_str());

	ILuint ImageName;
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);

	unsigned int textureNum = 0u;

	// Loading image
	if (ilLoadImage(exportedFile))
	{
		ILinfo imageInfo;
		iluGetImageInfo(&imageInfo);
		texture->SetWidth(imageInfo.Width);
		texture->SetHeight(imageInfo.Height);
		// TODO: If extension is TGA you'll have to flip this concrete texture
		//if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		//	iluFlipImage();

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
			App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->SetTexture(texture);
		}
		else
		{
			App->scene->goSelected->CreateComponent(COMPONENT_TYPE::COMPONENT_MATERIAL, "Material");
			App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->SetTexture(texture);
		}
	}
	if (texture->GetWidth() == 0)
		return nullptr;
	return texture;
}