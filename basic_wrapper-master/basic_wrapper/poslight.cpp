/*
 poslight.cpp
 Demonstrates a positional light with attenuation
 Displays a cube and a sphere and a small sphere to show the light position
 Includes controls to move the light source and rotate the view
 Iain Martin October 2014
*/

/* Link to static libraries, could define these as linker inputs in the project settings instead
if you prefer */
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "soil.lib")

/* Include the header to the GLFW wrapper class which
   also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>
#include "Cylinder.h"
#include "Tetrahedron.h"
#include "Sphere.h"
#include "Cube.h"

//include soil
#include "soil.h"


/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

Cylinder cylinder;
Sphere sphere;
Tetrahedron tetrahedron;
Cube cube;

/* Define buffer object indices */

GLuint program;		/* Identifier for the shader prgoram */
GLuint vao;			/* Vertex array (Containor) object. This is the index of the VAO that will be the container for
					   our buffer objects */

GLuint colourmode;	/* Index of a uniform to switch the colour mode in the vertex shader
					  I've included this to show you how to pass in an unsigned integer into
					  your vertex shader. */
GLuint emitmode;

GLuint texID;

/* Position and view globals */
GLfloat angle_x, angle_inc_x, x, scale, z, y, vx, vy, vz;
GLfloat angle_y, angle_inc_y, angle_z, angle_inc_z;
GLuint drawmode;			// Defines drawing mode of sphere as points, lines or filled polygons

GLfloat light_x, light_y, light_z;

/* Uniforms*/
GLuint modelID, viewID, projectionID, lightposID, normalmatrixID;
GLuint colourmodeID, emitmodeID;

GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/

/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw)
{
	/* Set the object transformation controls to their initial values */
	x = 0.05f;
	y = 0;
	z = 0;
	vx = 0; vx = 0, vz = 4.f;
	light_x = .5; light_y = .5; light_z = .5;
	angle_x = angle_y = angle_z = 0;
	angle_inc_x = angle_inc_y = angle_inc_z = 0;
	scale = 1.f;
	aspect_ratio = 1.3333f;
	colourmode = 0; emitmode = 0;


	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	
	cylinder.init();
	sphere.init();
	tetrahedron.init();

	/* Load and build the vertex and fragment shaders */
	try
	{
		program = glw->LoadShader("poslight.vert", "poslight.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	try
	{
		/* Not actually needed if using one texture at a time */
		glActiveTexture(GL_TEXTURE0);

		/* load an image file directly as a new OpenGL texture */
		texID = SOIL_load_OGL_texture("img.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

		/* check for an error during the load process */
		if (texID == 0)
		{
			printf("TexID SOIL loading error: '%s'\n", SOIL_last_result());
		}

		/* Standard bit of code to enable a uniform sampler for our texture */
		int loc = glGetUniformLocation(program, "tex1");
		if (loc >= 0) glUniform1i(loc, 0);
	}
	catch (std::exception &e)
	{
		printf("\nImage file loading failed.");
	}

	/* Define uniforms to send to vertex shader */
	modelID = glGetUniformLocation(program, "model");
	colourmodeID = glGetUniformLocation(program, "colourmode");
	emitmodeID = glGetUniformLocation(program, "emitmode");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	lightposID = glGetUniformLocation(program, "lightpos");
	normalmatrixID = glGetUniformLocation(program, "normalmatrix");
}

/* Called to update the display. Note that this function is called in the event loop in the wrapper
   class because we registered display as a callback function */
void display()
{
	/* Define the background colour */
	glClearColor(0.0f, 0.5f, 0.6f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	/* Make the compiled shader program current */
	glUseProgram(program);

	 //Define the model transformations for the cube
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x+0.5, y, z));
	model = glm::scale(model, glm::vec3(scale, scale, scale));//scale equally in all axis
	model = glm::rotate(model, -angle_x, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model = glm::rotate(model, -angle_y, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model = glm::rotate(model, -angle_z, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis

	 //Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 100.0f);

	 //Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 4), // Camera is at (0,0,4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	 //Apply rotations to the view position
	View = glm::rotate(View, -vx, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	View = glm::rotate(View, -vy, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	View = glm::rotate(View, -vz, glm::vec3(0, 0, 1));

	 //Define the light position and transform by the view matrix
	glm::vec4 lightpos = View *  glm::vec4(light_x, light_y, light_z, 1.0);

	 //Define the normal matrix
	glm::mat3 normalmatrix = glm::transpose(glm::inverse(glm::mat3(View * model)));

	 //Send our uniforms variables to the currently bound shader,
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	glUniform1ui(colourmodeID, colourmode);
	glUniform1ui(emitmodeID, emitmode);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));

	/* Draw our cube*/
	//cube.makeCube();

	/* Define the model transformations for our sphere */
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::scale(model, glm::vec3(scale/2.f, scale/2.0f, scale/20.f));//scale equally in all axis
	model = glm::rotate(model, -0.f, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model = glm::rotate(model, -90.f, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model = glm::rotate(model, -90.0f, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

	/* Draw our cylinder */
	cylinder.drawCylinder();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, -0.05));
	model = glm::scale(model, glm::vec3(scale / 1.5f, scale / 1.5f, scale / 20.f));//scale equally in all axis
	model = glm::rotate(model, -0.f, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model = glm::rotate(model, -90.f, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model = glm::rotate(model, -90.0f, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

	/* Draw our cylinder */
	cylinder.drawCylinder();
	//tetrahedron.drawTetrahedron();
	//drawSphere();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 0.05));
	model = glm::scale(model, glm::vec3(scale / 25.f, scale / 25.f, scale / 25.f));//scale equally in all axis
	model = glm::rotate(model, -0.f, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model = glm::rotate(model, -90.f, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model = glm::rotate(model, -90.0f, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

	/* Draw our cylinder */
	cylinder.drawCylinder();
	//tetrahedron.drawTetrahedron();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 0.06));
	model = glm::scale(model, glm::vec3(scale / 20.f, scale / 1.5f, scale / 20.f));//scale equally in all axis
	model = glm::rotate(model, -90.f, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model = glm::rotate(model, -90.f, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model = glm::rotate(model, -90.0f, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

	/* Draw our cylinder */
	tetrahedron.drawTetrahedron();


	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 0.06));
	model = glm::scale(model, glm::vec3(scale / 20.f, scale / 1.5f, scale / 20.f));//scale equally in all axis
	model = glm::rotate(model, -270.f, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model = glm::rotate(model, -270.f, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model = glm::rotate(model, -270.0f, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

	/* Draw our cylinder */
	tetrahedron.drawTetrahedron();
	//tetrahedron.drawTetrahedron();
	//drawSphere();

	/* Draw a small sphere in the lightsource position to visually represent the light source */
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(light_x, light_y, light_z));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f)); // make a small sphere
	normalmatrix = glm::transpose(glm::inverse(glm::mat3(View * model)));
	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

	/* Draw our lightposition sphere */
	emitmode = 1;
	glUniform1ui(emitmodeID, emitmode);
	sphere.drawSphere();
	//drawSphere();
	emitmode = 0;

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	/* Modify our animation variables */
	angle_x += angle_inc_x;
	angle_y += angle_inc_y;
	angle_z += angle_inc_z;
}

/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = ((float)w / 640.f*4.f) / ((float)h / 480.f*3.f);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	/* Enable this call if you want to disable key responses to a held down key*/
	//if (action != GLFW_PRESS) return;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == 'Q') angle_inc_x -= 0.05f;
	if (key == 'W') angle_inc_x += 0.05f;
	if (key == 'E') angle_inc_y -= 0.05f;
	if (key == 'R') angle_inc_y += 0.05f;
	if (key == 'T') angle_inc_z -= 0.05f;
	if (key == 'Y') angle_inc_z += 0.05f;
	if (key == 'A') scale -= 0.02f;
	if (key == 'S') scale += 0.02f;
	if (key == 'Z') x -= 0.05f;
	if (key == 'X') x += 0.05f;
	if (key == 'C') y -= 0.05f;
	if (key == 'V') y += 0.05f;
	if (key == 'B') z -= 0.05f;
	if (key == 'N') z += 0.05f;
	if (key == '1') light_x -= 0.05f;
	if (key == '2') light_x += 0.05f;
	if (key == '3') light_y -= 0.05f;
	if (key == '4') light_y += 0.05f;
	if (key == '5') light_z -= 0.05f;
	if (key == '6') light_z += 0.05f;
	if (key == '7') vx -= 1.f;
	if (key == '8') vx += 1.f;
	if (key == '9') vy -= 1.f;
	if (key == '0') vy += 1.f;
	if (key == 'O') vz -= 1.f;
	if (key == 'P') vz += 1.f;

	if (key == 'M' && action != GLFW_PRESS)
	{
		colourmode = !colourmode;
		std::cout << "colourmode=" << colourmode << std::endl;
	}

	/* Cycle between drawing vertices, mesh and filled polygons */
	if (key == 'N' && action != GLFW_PRESS)
	{
		drawmode ++;
		if (drawmode > 2) drawmode = 0;
	}

}



/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(1024, 768, "Gregor Whyte - Graphics Assignment 1");;

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}





