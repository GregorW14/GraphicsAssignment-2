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



#include "Tetrahedron.h"
#include "Common.h"




Tetrahedron::Tetrahedron()
{
}


Tetrahedron::~Tetrahedron()
{
}

void Tetrahedron::init()
{
	numtetravertices = 12;
	defineTetrahedron();
}



// Define vertex buffer object names (e.g as globals)
//GLuint tetra_buffer_normals, tetra_buffer_vertices, tetra_buffer_colours;

/* function prototypes */
//void defineTetrahedron();
//void drawTetrahedron();

/* Create the tetrahedron before the event loop (e.g. in init()) */
//defineTetrahedron();

/* Draw the object inside the display function */
//drawTetrahedron();


/*
Define the vertices, normals and indices for a tetrahedron
which has side lengths of 1 and sits on the plane y=0. It is not centred on the origin
A = (0, 1 / sqrt(3), 0)
B = (0, 0, -1 / 2sqrt(3))
C = (-0.5, 0, 1 / 2sqrt(3))
D = (0.5, 0, 1 / 2sqrt(3))
Triangles: ADC, ABD, DBC & ACB
*/
void Tetrahedron::defineTetrahedron()
{
	glm::vec3 tetra_normals[12];	// Array for normals for flat shaded tetrahedorn

									// Define vertices as glm:vec3 type to make it easier to calculate normals
	glm::vec3 tetra_vertices[] = {
		glm::vec3(0, 0.577f, 0), glm::vec3(-0.5f, 0, 0.289f), glm::vec3(0.5f, 0, 0.289f),
		glm::vec3(0, 0.577f, 0), glm::vec3(0.5f, 0, 0.289f), glm::vec3(0, 0, -0.289f),
		glm::vec3(0.5f, 0, 0.289f), glm::vec3(-0.5f, 0, 0.289f), glm::vec3(0, 0, -0.289f),
		glm::vec3(0, 0.577f, 0), glm::vec3(0, 0, -0.289f), glm::vec3(-0.5f, 0, 0.289f)
	};

	/* Specify the vertex buffer */
	/* Note how we pass the array of glm::vec3. The data gets copied here so it's ok that
	terta_vertices is local */
	glGenBuffers(1, &tetra_buffer_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, tetra_buffer_vertices);
	glBufferData(GL_ARRAY_BUFFER, numtetravertices * sizeof(glm::vec3), &tetra_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Define twelve colours for the four flat shaded object */
	GLfloat tetra_colours[] = {
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	/* Specify the colour buffer */
	glGenBuffers(1, &tetra_buffer_colours);
	glBindBuffer(GL_ARRAY_BUFFER, tetra_buffer_colours);
	glBufferData(GL_ARRAY_BUFFER, numtetravertices * sizeof(GLfloat) * 4, &tetra_colours[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Calculate the normals for each triangle, then set each set of three normals to be the same
	// for flat shading
	for (int v = 0; v < numtetravertices; v += 3)
	{
		glm::vec3 normal = glm::cross(tetra_vertices[v + 1] - tetra_vertices[v],
			tetra_vertices[v + 2] - tetra_vertices[v]);
		tetra_normals[v] = tetra_normals[v + 1] = tetra_normals[v + 2] = normal;
	}

	/* Define a buffer of the vertex normals */
	glGenBuffers(1, &tetra_buffer_normals);
	glBindBuffer(GL_ARRAY_BUFFER, tetra_buffer_normals);
	glBufferData(GL_ARRAY_BUFFER, numtetravertices * sizeof(glm::vec3), &tetra_normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* Draws the sphere from the previously defined vertex and index buffers */
void Tetrahedron::drawTetrahedron()
{
	/* Bind the vertices */
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, tetra_buffer_vertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* Bind the colours */
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, tetra_buffer_colours);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* Bind the normals */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, tetra_buffer_normals);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Enable this line to show model in wireframe
	if (drawmode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//if (drawmode == 2)
	//{
	//	// Draw the vertices
	//	glPointSize(3.f);  // Set the point size when drawing vertices
	//	glDrawArrays(GL_POINTS, 0, numtetravertices);
	//}
	//else
	//{
		// Draw the triangles
		glDrawArrays(GL_TRIANGLES, 0, numtetravertices);
	//}
}
