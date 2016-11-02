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

#include "Cylinder.h"




Cylinder::Cylinder()
{

}

Cylinder::~Cylinder()
{

}

void Cylinder::init()
{
	numperdisk = 40;

	makeCylinderVBO(numperdisk);
}


/* Make a cylinder from two triangle fans (one at each pole) and triangle strips along latitudes */
/* This version uses indexed vertex buffers for both the fans at the poles and the latitude strips */
GLuint Cylinder::makeCylinderVBO(GLuint numperdisk)
{
	GLuint i, j;
	/* Calculate the number of vertices required in cylinder */
	GLuint numvertices = 2 + 2 * numperdisk;
	GLfloat* pVertices = new GLfloat[numvertices * 3];
	GLfloat* pNormals = new GLfloat[numvertices * 3];
	GLfloat* pColours = new GLfloat[numvertices * 4];
	makeCylinder(pVertices, pNormals, numperdisk);

	/* Define colours as the x,y,z components of the cylinder vertices */
	for (i = 0; i < numvertices; i++)
	{
		pColours[i * 4] = pVertices[i * 3];
		pColours[i * 4 + 1] = pVertices[i * 3 + 1];
		pColours[i * 4 + 2] = pVertices[i * 3 + 2];
		pColours[i * 4 + 3] = 1.f;
	}

	/* Generate the vertex buffer object */
	glGenBuffers(1, &cylinderBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numvertices * 3, pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &cylinderNormals);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 3, pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the colours in a buffer object */
	glGenBuffers(1, &cylinderColours);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 4, pColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Calculate the number of indices in our index array and allocate memory for it */
	GLuint numindices = ((numperdisk * 2) + 2) + ((numperdisk + 2) * 2);
	GLuint* pindices = new GLuint[numindices];

	// fill "indices" to define triangle strips
	GLuint index = 0;		// Current index

							// Define indices for the first triangle fan for one pole
	for (i = 0; i < numperdisk + 1; i++)
	{
		pindices[index++] = i;
	}
	pindices[index++] = 1;	// Join last triangle in the triangle fan

	GLuint start = 1;		// Start index for each latitude row
		for (i = 0; i < numperdisk; i++)
		{
			pindices[index++] = start + i;
			pindices[index++] = start + i + numperdisk;
		}
		pindices[index++] = start; // close the triangle strip loop by going back to the first vertex in the loop
		pindices[index++] = start + numperdisk; // close the triangle strip loop by going back to the first vertex in the loop



	// Define indices for the last triangle fan for the south pole region
	for (i = numvertices - 1; i > (numvertices - numperdisk - 2); i--)
	{
		pindices[index++] = i;
	}
	pindices[index] = numvertices - 2;	// Tie up last triangle in fan

										// Generate a buffer for the indices
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete pindices;
	delete pColours;
	delete pVertices;
	return numvertices;
}


/* Define the vertex positions for a cylinder. The array of vertices must have previosuly
been created.
*/
void  Cylinder::makeCylinder(GLfloat *pVertices, GLfloat *pNormals, GLuint numperdisk)
{
	GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;
	GLuint vnum = 0;
	GLuint r = 1;
	GLfloat x, y, z, angle_radians;


	/* Define north pole */
	pVertices[0] = 0; pVertices[1] = 1; pVertices[2] = 0;
	pNormals[0] = 0; pNormals[1] = 1; pNormals[2] = 0;
	vnum++;

	GLfloat anglestep = 360.f / numperdisk;

	/* Define vertices along latitude lines */
	
		for (GLfloat angle = -180.f; angle < 180.f; angle += anglestep)
		{
			angle_radians = angle * DEG_TO_RADIANS;

			if (angle != 270 && angle != 90 && angle != -270 && angle != -90)
				x = r * cos(angle_radians);
			else
				x = 0;
			y = 1;
			//y = cos(lat_radians) * sin(lon_radians);
			if (angle != 360 && angle != 180 && angle != -360 && angle != -180)
				z = r * sin(angle_radians);
			else
				z = 0;

			/* Define the vertex */
			pVertices[vnum * 3] = x; pVertices[vnum * 3 + 1] = y; pVertices[vnum * 3 + 2] = z;
			vnum++;
		}

		for (GLfloat angle = -180.f; angle < 180.f; angle += anglestep)
		{
			angle_radians = angle * DEG_TO_RADIANS;

			if (angle != 270 && angle != 90 && angle != -270 && angle != -90)
				x = r * cos(angle_radians);
			else
				x = 0;
			y = -1;
			//y = cos(lat_radians) * sin(lon_radians);
			if (angle != 360 && angle != 180 && angle != -360 && angle != -180)
				z = r * sin(angle_radians);
			else
				z = 0;
			/* Define the vertex */
			pVertices[vnum * 3] = x; pVertices[vnum * 3 + 1] = y; pVertices[vnum * 3 + 2] = z;
			vnum++;
		}
	
	/* Define south pole */
	pVertices[vnum * 3] = 0; pVertices[vnum * 3 + 1] = -1; pVertices[vnum * 3 + 2] = 0;
	pNormals[vnum * 3] = 0; pNormals[vnum * 3 + 1] = -1; pNormals[vnum * 3 + 2] = 0;

	for (int i = 0; i < vnum; i++)
	{
		std::cout << i << " - " << pVertices[i * 3] << ", " << pVertices[i * 3 + 1] << ", " << pVertices[i * 3 + 2]  << std::endl;
	}
}

/* Draws the cylinder form the previously defined vertex and index buffers */
void Cylinder::drawCylinder()
{
	GLuint i;

	/* Draw the vertices as GL_POINTS */
	glBindBuffer(GL_ARRAY_BUFFER, cylinderBufferObject);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	/* Bind the cylinder normals */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderNormals);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* Bind the cylinder colours */
	glBindBuffer(GL_ARRAY_BUFFER, cylinderColours);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glPointSize(3.f);

	// Enable this line to show model in wireframe
	//if (drawmode == 1)
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//else
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//if (drawmode == 2)
	//{
	//glDrawArrays(GL_POINTS, 0, numcylindervertices);
	//}
	///else
	//{
	/* Bind the indexed vertex buffer */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	/* Draw the north pole regions as a triangle  */
	glDrawElements(GL_TRIANGLE_FAN, numperdisk + 2, GL_UNSIGNED_INT, (GLvoid*)(0));

	/* Calculate offsets into the indexed array. Note that we multiply offsets by 4
	because it is a memory offset the indices are type GLuint which is 4-bytes */
	GLuint lat_offset_jump = (numperdisk * 2) + 2;
	GLuint lat_offset_start = numperdisk + 2;
	GLuint lat_offset_current = lat_offset_start * 4;

	/* Draw the triangle strips of latitudes */

		glDrawElements(GL_TRIANGLE_STRIP, numperdisk * 2 + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));
		lat_offset_current += (lat_offset_jump * 4);

	/* Draw the south pole as a triangle fan */
	glDrawElements(GL_TRIANGLE_FAN, numperdisk + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));
	//}
}

