#pragma once
class Cube
{

	GLuint cubeBufferObject, cubeColours, cubeNormals;
	GLuint elementbuffer;

	GLuint numlats, numlongs;
public:
	Cube();
	~Cube();
	void init();
	void makeCube();
	void drawCube();
};

