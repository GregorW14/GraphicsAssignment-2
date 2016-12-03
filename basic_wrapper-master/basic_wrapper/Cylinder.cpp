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
#include "Common.h"




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
	makeCylinderTopVBO(numperdisk);
	makeCylinderBottomVBO(numperdisk);
}


/* Make a cylinder from two triangle fans (one at each pole) and triangle strips along latitudes */
/* This version uses indexed vertex buffers for both the fans at the poles and the latitude strips */
GLuint Cylinder::makeCylinderVBO(GLuint numperdisk)
{
	GLuint i, j;
	/* Calculate the number of vertices required in cylinder */
	GLuint numvertices = 2 + 2 * numperdisk;
	GLfloat* pVertices = new GLfloat[numvertices * 3];
	GLfloat* pTextures = new GLfloat[numvertices * 2];
	GLfloat* pNormals = new GLfloat[numvertices * 3];
	GLfloat* pColours = new GLfloat[numvertices * 4];
	makeCylinder(pVertices, pNormals, pTextures, numperdisk);

	/* Define colours as the x,y,z components of the cylinder vertices */
	for (i = 0; i < numvertices; i++)
	{
		pColours[i * 4] = 1.0f;
		pColours[i * 4 + 1] = 1.0f;
		pColours[i * 4 + 2] = 1.0f;
		pColours[i * 4 + 3] = 1.0f;
	}

	/* Generate the vertex buffer object */
	glGenBuffers(1, &cylinderBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numvertices * 3, pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &cylinderNormals);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 3, pNormals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the colours in a buffer object */
	glGenBuffers(1, &cylinderColours);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 4, pColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &cylinderTextures);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderTextures);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 2 + 2, pTextures, GL_STATIC_DRAW);
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

	delete[] pindices;
	delete[] pColours;
	delete[] pVertices;
	delete[] pNormals;
	return numvertices;
}

GLuint Cylinder::makeCylinderTopVBO(GLuint numperdisk)
{
	GLuint i, j;
	/* Calculate the number of vertices required in cylinder */
	GLuint numvertices = numperdisk + 1;
	GLfloat* pTopVertices = new GLfloat[numvertices * 3];
	GLfloat* pTopNormals = new GLfloat[numvertices * 3];
	GLfloat* pTopColours = new GLfloat[numvertices * 4];
	GLfloat* pTopTextures = new GLfloat[numvertices * 2];
	makeCylinderTop(pTopVertices, pTopNormals, pTopTextures, numperdisk);

	/* Define colours as the x,y,z components of the cylinder vertices */
	for (i = 0; i < numvertices; i++)
	{
		pTopColours[i * 4] = 1.0f;
		pTopColours[i * 4 + 1] = 1.0f;
		pTopColours[i * 4 + 2] = 1.0f;
		pTopColours[i * 4 + 3] = 1.0f;
	}


	/* Generate the vertex buffer object */
	glGenBuffers(1, &cylinderTopBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderTopBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numvertices * 3, pTopVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &cylinderTopNormals);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderTopNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 3, pTopNormals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the colours in a buffer object */
	glGenBuffers(1, &cylinderTopColours);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderTopColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 4, pTopColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the colours in a buffer object */
	glGenBuffers(1, &cylinderTopTextures);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderTopTextures);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 2, pTopTextures, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Calculate the number of indices in our index array and allocate memory for it */
	GLuint numindices = ((numperdisk * 2) + 2);
	GLuint* pindices = new GLuint[numindices];

	// fill "indices" to define triangle strips
	GLuint index = 0;		// Current index

							// Define indices for the first triangle fan for one pole
	for (i = 0; i < numperdisk + 1; i++)
	{
		pindices[index++] = i;
	}
	pindices[index++] = 1;	// Join last triangle in the triangle fan

	// Generate a buffer for the indices
	glGenBuffers(1, &elementbufferTop);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferTop);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] pindices;
	delete[] pTopColours;
	delete[] pTopVertices;
	delete[] pTopNormals;
	return numvertices;
}

GLuint Cylinder::makeCylinderBottomVBO(GLuint numperdisk)
{
	GLuint i, j;
	/* Calculate the number of vertices required in cylinder */
	GLuint numvertices = numperdisk + 1;
	GLfloat* pBottomVertices = new GLfloat[numvertices * 3];
	GLfloat* pBottomNormals = new GLfloat[numvertices * 3];
	GLfloat* pBottomColours = new GLfloat[numvertices * 4];
	GLfloat* pBottomTextures = new GLfloat[numvertices * 2];
	makeCylinderBottom(pBottomVertices, pBottomNormals, pBottomTextures, numperdisk);

	/* Define colours as the x,y,z components of the cylinder vertices */
	for (i = 0; i < numvertices; i++)
	{
		pBottomColours[i * 4] = 1.0f;
		pBottomColours[i * 4 + 1] = 1.0f;
		pBottomColours[i * 4 + 2] = 1.0f;
		pBottomColours[i * 4 + 3] = 1.0f;
	}

	/* Generate the vertex buffer object */
	glGenBuffers(1, &cylinderBottomBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numvertices * 3, pBottomVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &cylinderBottomNormals);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 3, pBottomNormals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the colours in a buffer object */
	glGenBuffers(1, &cylinderBottomColours);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 4, pBottomColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &cylinderBottomTextures);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomTextures);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 2, pBottomTextures, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Calculate the number of indices in our index array and allocate memory for it */
	GLuint numindices = ((numperdisk * 2) + 2);
	GLuint* pindices = new GLuint[numindices];

	// fill "indices" to define triangle strips
	GLuint index = 0;		// Current index

							// Define indices for the first triangle fan for one pole
	for (i = 0; i < numperdisk + 1; i++)
	{
		pindices[index++] = i;
	}
	pindices[index++] = 1;	// Join last triangle in the triangle fan

	// Generate a buffer for the indices
	glGenBuffers(1, &elementbufferBottom);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferBottom);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] pindices;
	delete[] pBottomColours;
	delete[] pBottomVertices;
	delete[] pBottomNormals;
	return numvertices;
}


/* Define the vertex positions for a cylinder. The array of vertices must have previosuly
been created.
*/
void  Cylinder::makeCylinder(GLfloat *pVertices, GLfloat *pNormals, GLfloat *pTextures, GLuint numperdisk)
{
	GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;
	GLuint vnum = 0;
	GLuint r = 1;
	GLfloat x, y, z, angle_radians;
	GLuint tex;





	GLfloat anglestep = 360.f / numperdisk;

	
		for (GLfloat angle = 360.f; angle > 0; angle -= anglestep)
		{
			angle_radians = angle * DEG_TO_RADIANS;

			if (angle != 270 && angle != 90 && angle != -270 && angle != -90)
				x = r * cos(angle_radians);
			else
				x = 0;
			y = 2;
			if (angle != 360 && angle != 180 && angle != -360 && angle != -180)
				z = r * sin(angle_radians);
			else
				z = 0;

			/* Define the vertex */
			pVertices[vnum * 3] = x; pVertices[vnum * 3 + 1] = y; pVertices[vnum * 3 + 2] = z;
			pNormals[vnum * 3] = x; pNormals[vnum * 3 + 1] = 0; pNormals[vnum * 3 + 2] = z;
			pTextures[vnum * 2] = angle / 360.f; pTextures[vnum * 2 + 1] = 1;
			vnum++;
		}
		pVertices[vnum * 3] = pVertices[0]; pVertices[vnum * 3 + 1] = pVertices[1]; pVertices[vnum * 3 + 2] = pVertices[2];
		pNormals[vnum * 3] = pNormals[0]; pNormals[vnum * 3 + 1] = pNormals[1]; pNormals[vnum * 3 + 2] = pNormals[2];

		pTextures[vnum * 2] = 0;
		pTextures[vnum * 2 + 1] = 1;
		vnum++;


		for (GLfloat angle = 360.f; angle > 0; angle -= anglestep)
		{
			angle_radians = angle * DEG_TO_RADIANS;

			if (angle != 270 && angle != 90 && angle != -270 && angle != -90)
				x = r * cos(angle_radians);
			else
				x = 0;
			y = -2;
			if (angle != 360 && angle != 180 && angle != -360 && angle != -180)
				z = r * sin(angle_radians);
			else
				z = 0;
			/* Define the vertex */
			pVertices[vnum * 3] = x; pVertices[vnum * 3 + 1] = y; pVertices[vnum * 3 + 2] = z;
			pNormals[vnum * 3] = x; pNormals[vnum * 3 + 1] = 0; pNormals[vnum * 3 + 2] = z;
			pTextures[vnum * 2] = angle / 360.f; pTextures[vnum * 2 + 1] = 0;
			vnum++;
		}
		int index = numperdisk + 1;
		pVertices[vnum * 3] = pVertices[index * 3]; pVertices[vnum * 3 + 1] = pVertices[index * 3 + 1]; pVertices[vnum * 3 + 2] = pVertices[index * 3 + 2];
		pNormals[vnum * 3] = pNormals[index * 3]; pNormals[vnum * 3 + 1] = pNormals[index * 3 + 1]; pNormals[vnum * 3 + 2] = pNormals[index * 3 + 2];

		pTextures[vnum * 2] = 0;
		pTextures[vnum * 2 + 1] = 0;
		//vnum++;




}

void  Cylinder::makeCylinderTop(GLfloat *pTopVertices, GLfloat *pTopNormals, GLfloat *pTopTextures, GLuint numperdisk)
{
	GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;
	GLuint vnum = 0;
	GLuint r = 1;
	GLfloat x, y, z, angle_radians;


	/* Define north pole */
	pTopVertices[0] = 0; pTopVertices[1] = 2; pTopVertices[2] = 0;
	pTopNormals[0] = 0; pTopNormals[1] = 1; pTopNormals[2] = 0;
	pTopTextures[0] = 0.5; pTopTextures[1] = 0.5;
	vnum++;

	GLfloat anglestep = 360.f / numperdisk;

	//Code for top disk
	for (GLfloat angle = -180.f; angle < 180.f; angle += anglestep)
	{
		angle_radians = angle * DEG_TO_RADIANS;

		if (angle != 270 && angle != 90 && angle != -270 && angle != -90)
			x = r * cos(angle_radians);
		else
			x = 0;
		y = 2;
		//y = cos(lat_radians) * sin(lon_radians);
		if (angle != 360 && angle != 180 && angle != -360 && angle != -180)
			z = r * sin(angle_radians);
		else
			z = 0;

		/* Define the vertex */
		pTopVertices[vnum * 3] = x; pTopVertices[vnum * 3 + 1] = y; pTopVertices[vnum * 3 + 2] = z;
		pTopNormals[vnum * 3] = 0; pTopNormals[vnum * 3 + 1] = 1; pTopNormals[vnum * 3 + 2] = 0;
		pTopTextures[vnum * 2] = x*0.5 + 0.5; pTopTextures[vnum * 2 + 1] = z*0.5 + 0.5;

		vnum++;
	}
}

void  Cylinder::makeCylinderBottom(GLfloat *pBottomVertices, GLfloat *pBottomNormals, GLfloat *pBottomTextures, GLuint numperdisk)
{
	GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;
	GLuint vnum = 0;
	GLuint r = 1;
	GLfloat x, y, z, angle_radians;


	/* Define south pole */
	pBottomVertices[vnum * 3] = 0; pBottomVertices[vnum * 3 + 1] = -2; pBottomVertices[vnum * 3 + 2] = 0;
	pBottomNormals[vnum * 3] = 0; pBottomNormals[vnum * 3 + 1] = -1; pBottomNormals[vnum * 3 + 2] = 0;
	pBottomTextures[0] = 0.5; pBottomTextures[1] = 0.5;
	vnum++;

	GLfloat anglestep = 360.f / numperdisk;

	//Code for bottom disk
	for (GLfloat angle = -180.f; angle < 180.f; angle += anglestep)
	{
		angle_radians = angle * DEG_TO_RADIANS;

		if (angle != 270 && angle != 90 && angle != -270 && angle != -90)
			x = r * cos(angle_radians);
		else
			x = 0;
		y = -2;
		//y = cos(lat_radians) * sin(lon_radians);
		if (angle != 360 && angle != 180 && angle != -360 && angle != -180)
			z = r * sin(angle_radians);
		else
			z = 0;
		/* Define the vertex */
		pBottomVertices[vnum * 3] = x; pBottomVertices[vnum * 3 + 1] = y; pBottomVertices[vnum * 3 + 2] = z;
		pBottomNormals[vnum * 3] = 0; pBottomNormals[vnum * 3 + 1] = -1; pBottomNormals[vnum * 3 + 2] =0;
		pBottomTextures[vnum * 2] = x*0.5 + 0.5; pBottomTextures[vnum * 2 + 1] = z*0.5 + 0.5;
		vnum++;
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

	glBindBuffer(GL_ARRAY_BUFFER, cylinderTextures);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glPointSize(3.f);

	// Enable this line to show model in wireframe
	if (drawmode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//{
	/* Bind the indexed vertex buffer */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	/* Draw the north pole regions as a triangle  */
	//glDrawElements(GL_TRIANGLE_FAN, numperdisk + 2, GL_UNSIGNED_INT, (GLvoid*)(0));

	/* Calculate offsets into the indexed array. Note that we multiply offsets by 4
	because it is a memory offset the indices are type GLuint which is 4-bytes */
	GLuint lat_offset_jump = (numperdisk * 2) + 2;
	GLuint lat_offset_start = numperdisk + 2;
	GLuint lat_offset_current = lat_offset_start * 4;

	/* Draw the triangle strips of latitudes */

		glDrawElements(GL_TRIANGLE_STRIP, numperdisk * 2 + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));

	/* Draw the south pole as a triangle fan */
	//glDrawElements(GL_TRIANGLE_FAN, numperdisk + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));
	//}


	/* Draw the vertices as GL_POINTS */
		glBindBuffer(GL_ARRAY_BUFFER, cylinderTopBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		/* Bind the cylinder normals */
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, cylinderTopNormals);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Bind the cylinder colours */
		glBindBuffer(GL_ARRAY_BUFFER, cylinderTopColours);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, cylinderTopTextures);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3);

		glPointSize(3.f);

		// Enable this line to show model in wireframe
		if (drawmode == 1)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//if (drawmode == 2)
		//{
		//glDrawArrays(GL_POINTS, 0, numcylindervertices);
		//}
		///else
		//{
		/* Bind the indexed vertex buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferTop);

		/* Draw the north pole regions as a triangle  */
		glDrawElements(GL_TRIANGLE_FAN, numperdisk + 2, GL_UNSIGNED_INT, (GLvoid*)(0));


		/* Draw the vertices as GL_POINTS */
		glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		/* Bind the cylinder normals */
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomNormals);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Bind the cylinder colours */
		glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomColours);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomTextures);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glPointSize(3.f);

		// Enable this line to show model in wireframe
		if (drawmode == 1)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//if (drawmode == 2)
		//{
		//glDrawArrays(GL_POINTS, 0, numcylindervertices);
		//}
		///else
		//{
		/* Bind the indexed vertex buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferBottom);

		/* Draw the South pole regions as a triangle  */
		glDrawElements(GL_TRIANGLE_FAN, numperdisk + 2, GL_UNSIGNED_INT, (GLvoid*)(0));

}

