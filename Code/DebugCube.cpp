#include "Application.h"
#include "DebugCube.h"

DebugCube::DebugCube(math::float3 center, math::float3 resize)
{
	position = center;
	scale = resize;

	verticesSize = 24;
	verticesArray = new GLfloat[verticesSize]{
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f
	};

	vertices = 0u;
	glGenBuffers(1, (GLuint*) &(vertices));
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesSize, verticesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	indicesSize = 24;
	indicesArray = new unsigned int[indicesSize] {
		0,1,
		1,2,
		2,3,
		3,0,
		4,5,
		5,6,
		6,7,
		7,4,
		0,4,
		3,7,
		2,6,
		1,5
	};

	indices = 0u;
	glGenBuffers(1, (GLuint*) &(indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesSize, indicesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

DebugCube::~DebugCube()
{
	glDeleteBuffers(1, (GLuint*)&vertices);
	glDeleteBuffers(1, (GLuint*)&indices);
	if (verticesArray != nullptr)
		delete[] verticesArray;
	if (indicesArray != nullptr)
		delete[] indicesArray;
}


void DebugCube::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	glScalef(scale.x, scale.y, scale.z);

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glLineWidth(4);
	glColor3f(0,255,0);
	glDrawElements(GL_LINES, indicesSize, GL_UNSIGNED_INT, NULL);
	glColor3f(255, 255, 255);
	glLineWidth(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
}

void DebugCube::DirectRender(const math::float3* vertices, Color color)
{
	glPushMatrix();

	glColor3f(color.r, color.g, color.b);
	glLineWidth(3);

	glBegin(GL_LINES);

	glVertex3fv((const GLfloat*)&vertices[1]);
	glVertex3fv((const GLfloat*)&vertices[5]);
	glVertex3fv((const GLfloat*)&vertices[5]);
	glVertex3fv((const GLfloat*)&vertices[7]);
	glVertex3fv((const GLfloat*)&vertices[7]);
	glVertex3fv((const GLfloat*)&vertices[3]);
	glVertex3fv((const GLfloat*)&vertices[3]);
	glVertex3fv((const GLfloat*)&vertices[1]);

	glVertex3fv((const GLfloat*)&vertices[4]);
	glVertex3fv((const GLfloat*)&vertices[0]);
	glVertex3fv((const GLfloat*)&vertices[0]);
	glVertex3fv((const GLfloat*)&vertices[2]);
	glVertex3fv((const GLfloat*)&vertices[2]);
	glVertex3fv((const GLfloat*)&vertices[6]);
	glVertex3fv((const GLfloat*)&vertices[6]);
	glVertex3fv((const GLfloat*)&vertices[4]);

	glVertex3fv((const GLfloat*)&vertices[5]);
	glVertex3fv((const GLfloat*)&vertices[4]);
	glVertex3fv((const GLfloat*)&vertices[4]);
	glVertex3fv((const GLfloat*)&vertices[6]);
	glVertex3fv((const GLfloat*)&vertices[6]);
	glVertex3fv((const GLfloat*)&vertices[7]);
	glVertex3fv((const GLfloat*)&vertices[7]);
	glVertex3fv((const GLfloat*)&vertices[5]);

	glVertex3fv((const GLfloat*)&vertices[0]);
	glVertex3fv((const GLfloat*)&vertices[1]);
	glVertex3fv((const GLfloat*)&vertices[3]);
	glVertex3fv((const GLfloat*)&vertices[2]);

	glVertex3fv((const GLfloat*)&vertices[3]);
	glVertex3fv((const GLfloat*)&vertices[7]);
	glVertex3fv((const GLfloat*)&vertices[6]);
	glVertex3fv((const GLfloat*)&vertices[2]);

	glVertex3fv((const GLfloat*)&vertices[0]);
	glVertex3fv((const GLfloat*)&vertices[4]);
	glVertex3fv((const GLfloat*)&vertices[5]);
	glVertex3fv((const GLfloat*)&vertices[1]);

	glEnd();

	glLineWidth(1);

	glPopMatrix();
}