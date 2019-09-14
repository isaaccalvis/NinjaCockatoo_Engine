#ifndef __Mesh_H__
#define __Mesh_H__

#include "glew-2.1.0/include/GL/glew.h"
#include "MathGeoLib-1.5/src/MathGeoLib.h"

enum DRAW_TYPE_SHAPE
{
	TRIANGLE,
	QUAD
};

class Mesh
{
public:
	Mesh();
	~Mesh(); // TODO: DESTRUCTOR

	void Render();

	void SetPosition(math::float3 nPosition);
	void SetScale(math::float3 nScale);

	math::float3 GetPosition() const;
	math::float3 GetScale() const;


protected:
	int verticesSize = 0;
	unsigned int vertices = 0u;
	int indicesSize = 0;
	unsigned int indices = 0u;

	math::float3 position = {0,0,0};
	math::float3 scale = { 1,1,1 };

	DRAW_TYPE_SHAPE type_shape = DRAW_TYPE_SHAPE::TRIANGLE;
};

#endif