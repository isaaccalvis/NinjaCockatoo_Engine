#ifndef __Mesh_H__
#define __Mesh_H__

#include "glew-2.1.0/include/GL/glew.h"
#include "MathGeoLib-1.5/src/MathGeoLib.h"

// TODO : Think system to draw in quads ?

class Mesh
{
public:
	Mesh();
	~Mesh(); // TODO: DESTRUCTOR

	virtual void Render();

	void SetPosition(math::float3 nPosition);
	void SetScale(math::float3 nScale);

	math::float3 GetPosition() const;
	math::float3 GetScale() const;


protected:
	unsigned int vertices = 0u;
	unsigned int verticesSize = 0;
	GLfloat* verticesArray = nullptr;

	unsigned int indices = 0u;
	unsigned int indicesSize = 0;
	unsigned int* indicesArray = nullptr;

	math::float3 position = {0,0,0};
	math::float3 scale = { 1,1,1 };
};

#endif