#include "Application.h"
#include "mmgr/mmgr.h"

#include "Texture.h"

Texture::Texture()
{

}

Texture::Texture(const char* name)
{
	this->name = name;
}

Texture::~Texture()
{

}

void Texture::SetBufferPos(unsigned int nBufferPos)
{
	this->BufferPos = nBufferPos;
}

void Texture::SetWidth(int nWidth)
{
	this->width = nWidth;
}

void Texture::SetHeight(int nHeigh)
{
	this->height = nHeigh;
}

unsigned int Texture::GetBufferPos() const
{
	return BufferPos;
}

int Texture::GetWidth() const
{
	return width;
}

int Texture::GetHeight() const
{
	return height;
}