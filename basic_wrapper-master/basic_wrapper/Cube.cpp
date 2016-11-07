#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "Cube.h"
#include "Common.h"



Cube::Cube()
{
}


Cube::~Cube()
{
}



GLfloat vertexPositions[] =
{
	-0.25f, 0.25f, -0.25f,
	-0.25f, -0.25f, -0.25f,
	0.25f, -0.25f, -0.25f,

	0.25f, -0.25f, -0.25f,
	0.25f, 0.25f, -0.25f,
	-0.25f, 0.25f, -0.25f,

	0.25f, -0.25f, -0.25f,
	0.25f, -0.25f, 0.25f,
	0.25f, 0.25f, -0.25f,

	0.25f, -0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, -0.25f,

	0.25f, -0.25f, 0.25f,
	-0.25f, -0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,

	-0.25f, -0.25f, 0.25f,
	-0.25f, 0.25f, 0.25f,
	0.25f, 0.25f, 0.25f,

	-0.25f, -0.25f, 0.25f,
	-0.25f, -0.25f, -0.25f,
	-0.25f, 0.25f, 0.25f,

	-0.25f, -0.25f, -0.25f,
	-0.25f, 0.25f, -0.25f,
	-0.25f, 0.25f, 0.25f,

	-0.25f, -0.25f, 0.25f,
	0.25f, -0.25f, 0.25f,
	0.25f, -0.25f, -0.25f,

	0.25f, -0.25f, -0.25f,
	-0.25f, -0.25f, -0.25f,
	-0.25f, -0.25f, 0.25f,

	-0.25f, 0.25f, -0.25f,
	0.25f, 0.25f, -0.25f,
	0.25f, 0.25f, 0.25f,

	0.25f, 0.25f, 0.25f,
	-0.25f, 0.25f, 0.25f,
	-0.25f, 0.25f, -0.25f,
};

/* Manually specified colours for our cube */
float vertexColours[] = {
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,

	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,

	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,

	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,

	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,

	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
	0.3f, 0.0f, 0.0f, 1.0f,
};

/* Manually specified normals for our cube */
GLfloat normals[] =
{
	0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
	0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
	1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
	1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
	0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
	0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
	-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
	-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
	0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
	0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
	0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
	0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
};

void Cube::init()
{
	
}

void Cube::makeCube()
{
	/* Create the vertex buffer for the cube */
	glGenBuffers(1, &cubeBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, cubeBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the colours buffer for the cube */
	glGenBuffers(1, &cubeColours);
	glBindBuffer(GL_ARRAY_BUFFER, cubeColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColours), vertexColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the normals  buffer for the cube */
	glGenBuffers(1, &cubeNormals);
	glBindBuffer(GL_ARRAY_BUFFER, cubeNormals);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Cube::drawCube()
{
	/* Bind cube vertices. Note that this is in attribute index 0 */
	glBindBuffer(GL_ARRAY_BUFFER, cubeBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube colours. Note that this is in attribute index 1 */
	glBindBuffer(GL_ARRAY_BUFFER, cubeColours);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube normals. Note that this is in attribute index 2 */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, cubeNormals);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (drawmode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}






