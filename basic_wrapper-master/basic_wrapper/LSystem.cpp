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

#include "LSystem.h"
#include "Common.h"
#include "Cylinder.h"

#include <stack>





	/* Aspect ratio of the window defined in the reshape callback*/

char lstring[20] = "F[+F][-F]\0";
std::stack<glm::mat4> lsystem_transform;
glm::mat4 model;
GLuint modelID;
GLuint texID;
int treelevel;
Cylinder branch;


/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
LSystem::LSystem()
{

}

LSystem::~LSystem()
{

}
void LSystem::init()
{
	x = 0;
	y = 0;
	z = 0;
	branch.init();

}

/* Called to update the display. Note that this function is called in the event loop in the wrapper
   class because we registered display as a callback function */
void LSystem::display()
{

}

/* Draw a single branch */
/* Could draw a differtn shape for a leaf! */
void LSystem::drawBranch(int leaf)
{
	glm::mat4 transform = lsystem_transform.top(); 
	transform = transform * model;

	/* Draw our textured quad*/
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(transform[0][0]));
	glBindTexture(GL_TEXTURE_2D, texID);
	//glBindTexture(GL_TEXTURE_2D, texID);
	branch.drawCylinder();
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

/* Recursive function to draw an LSystem Tree*/
void LSystem::tree(int level, int leaves) {
	int current;
	current = 0;
	while (lstring[current])
	{
		switch (lstring[current])
		{
		case 'F':

			if (level == 0)
			{
				if (leaves && ((lstring[current + 1] == ']') || (lstring[current + 1] == 0)))
					drawBranch(0);
				else
					drawBranch(1);

				lsystem_transform.top() = glm::translate(lsystem_transform.top(), glm::vec3(0, 2.5f, 0));
			}
			else
			{
				if ((lstring[current + 1] == ']') || (lstring[current + 1] == 0))
					tree(level - 1, leaves);
				else
					tree(level - 1, 0);
			}

			break;
		case '[':
			lsystem_transform.push(glm::scale(lsystem_transform.top(), glm::vec3(0.7, 0.7, 0.7)));
			break;
		case ']':
			lsystem_transform.pop();
			break;
		case '+':
			// rotate 45 degrees
			lsystem_transform.top() = glm::rotate(lsystem_transform.top(), -55.f, glm::vec3(0, 0, 1.f));
			lsystem_transform.top() = glm::translate(lsystem_transform.top(), glm::vec3(1.0f, 0, 0));
			break;
		case '-':
			lsystem_transform.top() = glm::rotate(lsystem_transform.top(), 45.f, glm::vec3(0, 0, 1.f));
			lsystem_transform.top() = glm::translate(lsystem_transform.top(), glm::vec3(-1.0f, 0, 0));
			break;
		default:
			break;
		}
		current++;
	}
}

	void LSystem::draw(std::stack<glm::mat4> m, GLuint mID, GLuint level, GLuint textureID) {
		model = glm::mat4(1.0f);
		modelID = mID;
		lsystem_transform = m;
		texID = textureID;

		lsystem_transform.push(m.top());
		{
			lsystem_transform.push(lsystem_transform.top());
			lsystem_transform.top() = glm::translate(lsystem_transform.top(), glm::vec3(0, 0, 0.f));
			tree(level, 1);
			lsystem_transform.pop();


		}

	}
