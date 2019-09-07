#include "Application.h"
#include "ModuleMeshes.h"

#include "glew-2.1.0/include/GL/glew.h"

ModuleMeshes::ModuleMeshes(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleMeshes";
}

ModuleMeshes::~ModuleMeshes()
{}

bool ModuleMeshes::Init(JSON_Object* root_object)
{
	return true;
}

update_status ModuleMeshes::Update(float dt)
{
	int size = 3;
	int verticesSize = 24;
	GLfloat* vertices_array = new GLfloat[verticesSize]{
		0.0f * size, 0.0f * size, 0.0f * size,
		1.0f * size, 0.0f * size, 0.0f * size,
		1.0f * size, 1.0f * size, 0.0f * size,
		0.0f * size, 1.0f * size, 0.0f * size,
		0.0f * size, 0.0f * size, 1.0f * size,
		1.0f * size, 0.0f * size, 1.0f * size,
		1.0f * size, 1.0f * size, 1.0f * size,
		0.0f * size, 1.0f * size, 1.0f * size
	};

	unsigned int vertices = 0u;
	glGenBuffers(1, (GLuint*) &(vertices));
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, vertices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	int indicesSize = 36;
	uint* indices_array = new uint[indicesSize]{
		2,1,0,
		3,2,0,
		2,5,1,
		5,2,6,
		3,7,2,
		2,7,6,
		5,6,4,
		6,7,4,
		3,0,4,
		7,3,4,
		0,1,4,
		1,5,4
	};

	uint indices = 0u;
	glGenBuffers(1, (GLuint*) &(indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);

	return update_status::UPDATE_CONTINUE;
}

bool ModuleMeshes::CleanUp()
{
	return true;
}

bool ModuleMeshes::Save(JSON_Object* root_object)
{
	return true;
}

bool ModuleMeshes::Load(JSON_Object* root_object)
{
	return true;
}