#include "Grid.h"

Grid::Grid(int nScale)
{
	scale.x = nScale;
	scale.y = nScale;
	scale.z = nScale;

	verticesSize = 48;
	verticesArray = new GLfloat[verticesSize]{
		-2,0,-2,
		-1,0,-2,
		0,0,-2,
		1,0,-2,
		2,0,-2,
		2,0,-1,
		2,0,0,
		2,0,1,
		2,0,2,
		1,0,2,
		0,0,2,
		-1,0,2,
		-2,0,2,
		-2,0,1,
		-2,0,0,
		-2,0,-1
	};

	vertices = 0u;
	glGenBuffers(1, (GLuint*) &(vertices));
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesSize, verticesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	indicesSize = 20;
	indicesArray = new unsigned int[indicesSize] {
		0, 12, 11, 1, 2, 10, 9, 3, 4, 8, 8, 12, 13, 7, 6, 14, 15, 5, 4, 0
	};

	indices = 0u;
	glGenBuffers(1, (GLuint*) &(indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesSize, indicesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Grid::~Grid()
{

}

void Grid::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	glScalef(scale.x, scale.y, scale.z);

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glDrawElements(GL_LINES, indicesSize, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
}