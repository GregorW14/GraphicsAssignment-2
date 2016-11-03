#pragma once
class Cylinder
{
	GLuint numlats, numperdisk;	//Define the resolution of the sphere object

	GLuint cylinderBufferObject, cylinderNormals, cylinderColours, elementbuffer, cylinderTopBufferObject, cylinderTopNormals, cylinderTopColours, elementbufferTop, cylinderBottomBufferObject, cylinderBottomNormals, cylinderBottomColours, elementbufferBottom;

public:
	Cylinder();
	~Cylinder();
	void init();
	GLuint makeCylinderVBO(GLuint numperdisk);
	GLuint makeCylinderTopVBO(GLuint numperdisk);
	GLuint makeCylinderBottomVBO(GLuint numperdisk);
	void makeCylinder(GLfloat *pVertices, GLfloat *pNormals, GLuint numperdisk);
	void makeCylinderTop(GLfloat *pTopVertices, GLfloat *pTopNormals, GLuint numperdisk);
	void makeCylinderBottom(GLfloat *pBottomVertices, GLfloat *pBottomNormals, GLuint numperdisk);
	void drawCylinder();
};

