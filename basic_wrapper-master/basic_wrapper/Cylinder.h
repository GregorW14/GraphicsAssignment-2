#pragma once
class Cylinder
{
	GLuint numlats, numperdisk;	//Define the resolution of the sphere object

	GLuint cylinderBufferObject, cylinderNormals, cylinderColours, cylinderTextures, elementbuffer, cylinderTopBufferObject, cylinderTopNormals, cylinderTopColours, cylinderTopTextures, elementbufferTop, cylinderBottomBufferObject, cylinderBottomNormals, cylinderBottomColours, cylinderBottomTextures, elementbufferBottom;

public:
	Cylinder();
	~Cylinder();
	void init();
	GLuint makeCylinderVBO(GLuint numperdisk);
	GLuint makeCylinderTopVBO(GLuint numperdisk);
	GLuint makeCylinderBottomVBO(GLuint numperdisk);
	void makeCylinder(GLfloat *pVertices, GLfloat *pNormals, GLfloat *pTextures, GLuint numperdisk);
	void makeCylinderTop(GLfloat *pTopVertices, GLfloat *pTopNormals, GLfloat *pTopTextures, GLuint numperdisk);
	void makeCylinderBottom(GLfloat *pBottomVertices, GLfloat *pBottomNormals, GLfloat *pBottomTextures, GLuint numperdisk);
	void drawCylinder();
};

