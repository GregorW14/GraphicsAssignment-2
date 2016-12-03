#pragma once
#include <stack>

class LSystem
{
	GLfloat  x, y, z;

public:
	LSystem();
	~LSystem();
	void draw(std::stack<glm::mat4> m, GLuint mID, GLuint level, GLuint textureID);
	void tree(int level, int leaves);
	void drawBranch(int leaf);
	void display();
	void init();
};