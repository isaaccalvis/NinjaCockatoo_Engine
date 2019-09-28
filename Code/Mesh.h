#ifndef __Mesh_H__
#define __Mesh_H__

#include "Globals.h"
#include "ModuleTextures.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "MathGeoLib-1.5/src/MathGeoLib.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	virtual void Render();

	void SetPosition(const math::float3 nPosition);
	void SetScale(const math::float3 nScale);

	math::float3 GetPosition() const;
	math::float3 GetScale() const;

protected:
	// Vertices
	unsigned int vertices = 0u;
	unsigned int verticesSize = 0;
	GLfloat* verticesArray = nullptr;

	// Indices
	unsigned int indices = 0u;
	unsigned int indicesSize = 0;
	unsigned int* indicesArray = nullptr;
	
	// Texture
	unsigned int textureIndex = 0u;
	GLfloat* textureCoords = nullptr;
	Texture* texture = nullptr;

	// Transform
	math::float3 position = {0,0,0};
	//math::float3 rotation = { 0,0,0 }; 	// TODO: MAKE ROTATION
	math::float3 scale = { 1,1,1 };
};

#endif