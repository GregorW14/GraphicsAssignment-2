#pragma once
class Sphere
{
	GLuint numlats, numlongs;	//Define the resolution of the sphere object

	GLuint sphereBufferObject, sphereNormals, sphereColours, elementbuffer;

public:
	Sphere();
	~Sphere();
	void init();
	GLuint makeSphereVBO(GLuint numlats, GLuint numlongs);
	void makeUnitSphere(GLfloat *pVertices, GLuint numlats, GLuint numlongs);
	void drawSphere();
};

