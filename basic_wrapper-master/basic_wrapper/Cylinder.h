#pragma once
class Cylinder
{
	GLuint numlats, numperdisk;	//Define the resolution of the sphere object

	GLuint cylinderBufferObject, cylinderNormals, cylinderColours, elementbuffer;

public:
	Cylinder();
	~Cylinder();
	void init();
	GLuint makeCylinderVBO(GLuint numperdisk);
	void makeCylinder(GLfloat *pVertices, GLfloat *pNormals, GLuint numperdisk);
	void drawCylinder();
};

