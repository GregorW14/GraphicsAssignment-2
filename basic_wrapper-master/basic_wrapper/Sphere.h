#pragma once
class Sphere
{
	GLuint numlats, numlongs;	//Define the resolution of the sphere object

	GLuint sphereBufferObject, sphereNormals, sphereColours, sphereTexCoords, elementbuffer;

public:
	Sphere();
	~Sphere();
	void init();
	GLuint makeSphereVBO(GLuint numlats, GLuint numlongs);
	void makeUnitSphere(GLfloat *pVertices, GLfloat *pTexCoords, GLuint numlats, GLuint numlongs);
	void drawSphere();
};

