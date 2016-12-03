/*
 Gregor Whyte Graphics Assignment 1
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
#include "Common.h"
#include "terrain_object.h"
#include "points2.h"
#include "object_ldr.h"
#include "LSystem.h"
#include <stack>

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
LSystem lsystem;
GLuint level;

/* Define buffer object indices */

GLuint program[3];		/* Identifier for the shader program */
GLuint vao;			/* Vertex array (Containor) object. This is the index of the VAO that will be the container for
					   our buffer objects */

GLuint colourmode;	/* Index of a uniform to switch the colour mode in the vertex shader
					  I've included this to show you how to pass in an unsigned integer into
					  your vertex shader. */
GLuint emitmode;

GLuint textured;

terrain_object terrain;	// Create a global terrain object

/* Position and view globals */
GLfloat angle_x, angle_inc_x, x, scale, z, y, vx, vy, vz, camera_x, camera_y, camera_z, ry;
GLfloat angle_y, angle_inc_y, angle_z, angle_inc_z;
GLfloat pendulumswing = 0, pendulumswing_inc = 0.2f, pendulumswing_max = 10.f;
GLfloat minuteHandswing = 0, minuteHandswing_inc = 0.08f;
GLfloat hourHandswing = 0, hourHandswing_inc = minuteHandswing_inc/12;
GLuint drawmode;			// Defines drawing mode of sphere as points, lines or filled polygons

GLfloat light_x, light_y, light_z;
object_ldr bed;
object_ldr plants;

GLfloat point_sizeID;


/* Uniforms*/
GLuint modelID[3], viewID[3], projectionID[3], lightposID, normalmatrixID[3], texturedID[3];
GLuint colourmodeID[3], emitmodeID[3];

GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/

points2 *point_anim;
GLfloat speed;
GLfloat maxdist;
GLfloat point_size;		// Used to adjust point size in the vertex shader
GLboolean night;

GLuint textureID, textureID2, textureID3, textureID4, textureID5, textureID6, textureID7, textureID8, textureID9, textureID10, textureID11, textureID12;

GLuint numlats, numlongs;	//Define the resolution of the sphere object
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
	ry = -2.8;
	night = true;
	camera_x = 6;
	camera_y = 5;
	camera_z = 30;
	vx = -1.5; vx = 0, vz = 0;
	light_x = 1; light_y = 15; light_z = 0;
	angle_x = angle_y = angle_z = 0;
	angle_inc_x = angle_inc_y = angle_inc_z = 0;
	scale = 1.f;
	aspect_ratio = 1.3333f;
	colourmode = 0; emitmode = 0, textured = 1;



	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	
	cylinder.init();
	sphere.init();
	tetrahedron.init();
	cube.makeCube();
	lsystem.init();
	terrain.createTerrain(100, 100, 200.f, 200.f);
	terrain.createObject();

	bed.load_obj("bed.obj");
	bed.smoothNormals();
	bed.createObject();

	plants.load_obj("plants.obj");
	plants.smoothNormals();
	plants.createObject();

	/* Load and build the vertex and fragment shaders */
	try
	{
		program[0] = glw->LoadShader("main.vert", "main.frag");
		program[1] = glw->LoadShader("point_sprites.vert", "point_sprites.frag");
		program[2] = glw->LoadShader("skybox.vert", "skybox.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	try
	{
		/* load an image file directly as a new OpenGL texture */
		textureID = SOIL_load_OGL_texture("grass.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
			SOIL_FLAG_COMPRESS_TO_DXT);

		textureID2 = SOIL_load_OGL_texture("brick.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
			SOIL_FLAG_COMPRESS_TO_DXT);

		textureID3 = SOIL_load_OGL_texture("door.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
			SOIL_FLAG_COMPRESS_TO_DXT);

		textureID4 = SOIL_load_OGL_texture("brass.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
			SOIL_FLAG_COMPRESS_TO_DXT);

		textureID5 = SOIL_load_OGL_texture("woodroof.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
			SOIL_FLAG_COMPRESS_TO_DXT);

		textureID6 = SOIL_load_OGL_texture("moon.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
			SOIL_FLAG_COMPRESS_TO_DXT);

		textureID7 = SOIL_load_OGL_texture("carpet.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
			SOIL_FLAG_COMPRESS_TO_DXT);

		textureID8 = SOIL_load_OGL_texture("night.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
			SOIL_FLAG_COMPRESS_TO_DXT);
		textureID9 = SOIL_load_OGL_texture("skybox.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
			SOIL_FLAG_COMPRESS_TO_DXT);

		textureID10 = SOIL_load_OGL_texture("sun.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
			SOIL_FLAG_COMPRESS_TO_DXT);
		textureID11 = SOIL_load_OGL_texture("bark.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
			SOIL_FLAG_COMPRESS_TO_DXT);
		textureID12 = SOIL_load_OGL_texture("wood.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
			SOIL_FLAG_COMPRESS_TO_DXT);

		/* check for an error during the load process */
		if (textureID == 0)
		{
			printf("textureID SOIL loading error: '%s'\n", SOIL_last_result());
		}
		if (textureID2 == 0)
		{
			printf("textureID2 SOIL loading error: '%s'\n", SOIL_last_result());
		}
		if (textureID3 == 0)
		{
			printf("textureID3 SOIL loading error: '%s'\n", SOIL_last_result());
		}
		if (textureID4 == 0)
		{
			printf("textureID4 SOIL loading error: '%s'\n", SOIL_last_result());
		}
		if (textureID5 == 0)
		{
			printf("textureID5 SOIL loading error: '%s'\n", SOIL_last_result());
		}
		if (textureID6 == 0)
		{
			printf("textureID6 SOIL loading error: '%s'\n", SOIL_last_result());
		}
		if (textureID7 == 0)
		{
			printf("textureID7 SOIL loading error: '%s'\n", SOIL_last_result());
		}
		if (textureID8 == 0)
		{
			printf("textureID8 SOIL loading error: '%s'\n", SOIL_last_result());
		}
		if (textureID9 == 0)
		{
			printf("textureID9 SOIL loading error: '%s'\n", SOIL_last_result());
		}
		if (textureID10 == 0)
		{
			printf("textureID10 SOIL loading error: '%s'\n", SOIL_last_result());
		}
		if (textureID11 == 0)
		{
			printf("textureID11 SOIL loading error: '%s'\n", SOIL_last_result());
		}
		if (textureID12 == 0)
		{
			printf("textureID12 SOIL loading error: '%s'\n", SOIL_last_result());
		}

		// This is the location of the texture object (TEXTURE0), i.e. tex1 will be the name
		// of the sampler in the fragment shader
		int loc = glGetUniformLocation(program[0], "tex1");
		if (loc >= 0) glUniform1i(loc, 0);
	}
	catch (std::exception &e)
	{
		printf("\nImage file loading failed.");
	}



	/* Define uniforms to send to vertex shader */
	for (int i = 0; i < 3; i++)
	{
		glUseProgram(program[i]);
		modelID[i] = glGetUniformLocation(program[i], "model");
		colourmodeID[i] = glGetUniformLocation(program[i], "colourmode");
		viewID[i] = glGetUniformLocation(program[i], "view");
		projectionID[i] = glGetUniformLocation(program[i], "projection");
		emitmodeID[i] = glGetUniformLocation(program[i], "emitmode");
		normalmatrixID[i] = glGetUniformLocation(program[i], "normalmatrix");
		texturedID[i] = glGetUniformLocation(program[i], "textured");
	}
	lightposID = glGetUniformLocation(program[0], "lightpos");
	point_sizeID = glGetUniformLocation(program[1], "size");
	speed = 0.025f;
	maxdist = 10.f;
	point_anim = new points2(5000, maxdist, speed);
	point_anim->create();
	point_size = 5;

	// Enable gl_PointSize
	glEnable(GL_PROGRAM_POINT_SIZE);
}

/* Called to update the display. Note that this function is called in the event loop in the wrapper
   class because we registered display as a callback function */
void display()
{
	/* Define the background colour */
	glClearColor(0.0f, 0.5f, 0.6f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	/* Make the compiled shader program current */
	glUseProgram(program[0]);

	 //Define the model transformations for the cube
	std::stack<glm::mat4> model;
	model.push(glm::mat4(1.0f));
	//glm::mat4 model = glm::mat4(1.0f);
	model.top() = glm::translate(model.top(), glm::vec3(x+0.5, y, z));
	model.top() = glm::rotate(model.top(), -angle_x, glm::vec3(1, 0, 0)); 
	model.top() = glm::rotate(model.top(), -angle_y, glm::vec3(0, 1, 0)); 
	model.top() = glm::rotate(model.top(), -angle_z, glm::vec3(0, 0, 1)); 
	model.top() = glm::scale(model.top(), glm::vec3(scale, scale, scale));
	model.push(model.top());

	 //Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 300.0f);

	 //Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(camera_x, camera_y, camera_z), // Camera is at (0,0,4), in World Space
		glm::vec3(camera_x,camera_y, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	 //Apply rotations to the view position
	View = glm::rotate(View, -vx, glm::vec3(1, 0, 0)); 
	View = glm::rotate(View, -vy, glm::vec3(0, 1, 0)); 
	View = glm::rotate(View, -vz, glm::vec3(0, 0, 1));

	 //Define the light position and transform by the view matrix
	glm::vec4 lightpos = View *  glm::vec4(light_x, light_y, light_z, 1.0);

	 //Define the normal matrix
	glm::mat3 normalmatrix = glm::transpose(glm::inverse(glm::mat3(View * model.top())));

	 //Send our uniforms variables to the currently bound shader,


	//skybox
	glUseProgram(program[2]);
	model.push(model.top()); 
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(0, 0, 0));
		model.top() = glm::rotate(model.top(), -90.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 0.005f, scale / 0.005f, scale / 0.005f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[2], 1, GL_FALSE, &model.top()[0][0]);

		glUniform1ui(colourmodeID[2], colourmode);
		glUniformMatrix4fv(viewID[2], 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(projectionID[2], 1, GL_FALSE, &Projection[0][0]);

		
		if(night == true)
		glBindTexture(GL_TEXTURE_2D, textureID8);
		else
		glBindTexture(GL_TEXTURE_2D, textureID9);

		
		sphere.drawSphere();

		model.pop();
	}

	glUseProgram(program[0]);
	glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);
	glUniform1ui(colourmodeID[0], colourmode);
	glUniform1ui(emitmodeID[0], emitmode);
	glUniformMatrix4fv(viewID[0], 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID[0], 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));
	glUniform1ui(texturedID[0], textured);

	//Left Wall
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(0, 2, 0));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 1.f, scale / 0.15f, scale / 0.04f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);

		
		glBindTexture(GL_TEXTURE_2D, textureID2);

		
		cube.drawCube();
		model.pop();
	}

	//Right Wall
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(12, 2, 0));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 1.f, scale / 0.15f, scale / 0.04f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);

		
		glBindTexture(GL_TEXTURE_2D, textureID2);

		
		cube.drawCube();
		model.pop();
	}

	//Back wall
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(6, 2, -6));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -90.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 1.f, scale / 0.15f, scale / 0.0435f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);

		
		glBindTexture(GL_TEXTURE_2D, textureID2);


		
		cube.drawCube();
		model.pop();
	}




	//Front Wall
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(6, 2, 6));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -90.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 1.f, scale / 0.15f, scale / 0.0435f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);

		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);

		
		glBindTexture(GL_TEXTURE_2D, textureID2);

		cube.drawCube();
		model.pop();
	}

	//Floor
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(6, 0.25, 0));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -90.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 0.04f, scale / 2.f, scale / 0.04f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);

		
		glBindTexture(GL_TEXTURE_2D, textureID7);

		
		cube.drawCube();
		model.pop();
	}


	//Roof Bottom
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(6, 6.6+ry, 0));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -90.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 0.04f, scale / 2.f, scale / 0.04f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);
		
		glBindTexture(GL_TEXTURE_2D, textureID5);


		cube.drawCube();
		model.pop();
	}


	//Roof 2nd Bottom
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(6, 6.8+ry, 0));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -90.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 0.045f, scale / 2.f, scale / 0.045f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);

		glBindTexture(GL_TEXTURE_2D, textureID5);


		cube.drawCube();
		model.pop();
	}

	//Roof 3rd Bottom
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(6, 7+ry, 0));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -90.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 0.05f, scale / 2.f, scale / 0.05f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);


		glBindTexture(GL_TEXTURE_2D, textureID5);

		cube.drawCube();
		model.pop();
	}


	//Roof 4th Bottom
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(6, 7.2+ry, 0));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -90.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 0.055f, scale / 2.f, scale / 0.055f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);



		glBindTexture(GL_TEXTURE_2D, textureID5);


		cube.drawCube();
		model.pop();
	}

	glUseProgram(program[0]);

	//Bulb Holder
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(6, 6.5+ry, 0));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -90.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 7.f, scale / 4.f, scale / 7.f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);


		cylinder.drawCylinder();
		model.pop();
	}


	//Bulb
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(6, 6.1+ry, 0));
		model.top() = glm::scale(model.top(), glm::vec3(0.2f, 0.2f, 0.2f)); 
		normalmatrix = glm::transpose(glm::inverse(glm::mat3(View * model.top())));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);


		glUniform1ui(texturedID[0], 0);
		emitmode = 1;
		glUniform1ui(emitmodeID[0], emitmode);
		sphere.drawSphere();
		emitmode = 0;
		model.pop();
	}

	//Door
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(6, 1.2, 6));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 0.4f, scale / 0.3f, scale / 0.8f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);

		glUniform1ui(texturedID[0], 1);
		glUniform1ui(emitmodeID[0], emitmode);
		glBindTexture(GL_TEXTURE_2D, textureID3);


		cube.drawCube();
		model.pop();
	}

	//doorknob
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(6.5, 1.1, 6.375));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -90.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 10.f, scale / 10.f, scale / 10.f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);


		glBindTexture(GL_TEXTURE_2D, textureID4);

		sphere.drawSphere();
		model.pop();
	}

	//
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(6.5, 1.1, 5.625));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), 90.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 10.f, scale / 10.f, scale / 10.f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);


		glBindTexture(GL_TEXTURE_2D, textureID4);


		sphere.drawSphere();
		model.pop();
	}


	//bed
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(10.2, 0.35, -3.2));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -80.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale / 55.f, scale / 55.f, scale / 55.f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);

		/* Draw bed */
		glUniform1ui(texturedID[0], 0);
		bed.drawObject();
		model.pop();
	}


	//table leg
	model.push(model.top());
	{

		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(3, 0.5, -3.2));
		model.top() = glm::scale(model.top(), glm::vec3(0.1f, 0.3f, 0.1f)); 
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);


		/* Draw our lightposition sphere */
		glUniform1ui(texturedID[0], 1);
		glBindTexture(GL_TEXTURE_2D, textureID12);
		cylinder.drawCylinder();
		model.pop();
	}

	//table leg 2
	model.push(model.top());
	{

		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(1, 0.5, -3.2));
		model.top() = glm::scale(model.top(), glm::vec3(0.1f, 0.3f, 0.1f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);


		/* Draw our lightposition sphere */
		glUniform1ui(texturedID[0], 1);
		glBindTexture(GL_TEXTURE_2D, textureID12);
		cylinder.drawCylinder();
		model.pop();
	}

	//table leg 3
	model.push(model.top());
	{

		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(1, 0.5, -1));
		model.top() = glm::scale(model.top(), glm::vec3(0.1f, 0.3f, 0.1f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);


		/* Draw our lightposition sphere */
		glUniform1ui(texturedID[0], 1);
		glBindTexture(GL_TEXTURE_2D, textureID12);
		cylinder.drawCylinder();
		model.pop();
	}

	//table leg 4
	model.push(model.top());
	{

		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(3, 0.5, -1));
		model.top() = glm::scale(model.top(), glm::vec3(0.1f, 0.3f, 0.1f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);


		/* Draw our lightposition sphere */
		glUniform1ui(texturedID[0], 1);
		glBindTexture(GL_TEXTURE_2D, textureID12);
		cylinder.drawCylinder();
		model.pop();
	}

	//table surface
	model.push(model.top());
	{

		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(2, 1.1, -2.1));
		model.top() = glm::scale(model.top(), glm::vec3(5.f, 0.3f, 5.f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);


		/* Draw our lightposition sphere */
		glUniform1ui(texturedID[0], 1);
		glBindTexture(GL_TEXTURE_2D, textureID12);
		cube.drawCube();
		model.pop();
	}

	//terrain
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(0, 3.0, 0));
		normalmatrix = glm::transpose(glm::inverse(glm::mat3(View * model.top())));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);

		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);

		glUniform1ui(texturedID[0], 1);
		glBindTexture(GL_TEXTURE_2D, textureID);
		terrain.drawObject(drawmode);
		model.pop();
	}

	//plants
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(3, 0, 8));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -180.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(1.f, 1.f, 1.f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);

		glUniform1ui(texturedID[0], 0);
		plants.drawObject();
		model.pop();
	}

	//plants
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(9, 0, 8));
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::rotate(model.top(), -0.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -0.0f, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(1.f, 1.f, 1.f));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);

		glUniform1ui(texturedID[0], 0);
		plants.drawObject();
		model.pop();
	}

	//tree
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(0, 1.2, 12));
		model.top() = glm::rotate(model.top(), 30.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::scale(model.top(), glm::vec3(0.7f, 0.7f, 0.7f)); 
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);

		glUniform1ui(texturedID[0], 1);
		level = 6;
		lsystem.draw(model, modelID[0], level, textureID11);
		model.pop();
	}

	//tree
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(12, 1.2, 12));
		model.top() = glm::rotate(model.top(), 30.f, glm::vec3(0, 1, 0)); 
		model.top() = glm::scale(model.top(), glm::vec3(0.7f, 0.7f, 0.7f)); 
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);

		glUniform1ui(texturedID[0], 1);
		level = 5;
		lsystem.draw(model, modelID[0], level, textureID11);
		model.pop();
	}



	//  lightsource 
	model.push(model.top());
	{

		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(light_x, light_y, light_z));
		model.top() = glm::rotate(model.top(), 90.f, glm::vec3(1, 0, 0)); 
		model.top() = glm::scale(model.top(), glm::vec3(1.f, 1.f, 1.f)); 
		normalmatrix = glm::transpose(glm::inverse(glm::mat3(View * model.top())));
		glUniformMatrix3fv(normalmatrixID[0], 1, GL_FALSE, &normalmatrix[0][0]);
		glUniformMatrix4fv(modelID[0], 1, GL_FALSE, &model.top()[0][0]);


		/* Draw our lightposition sphere */
		glUniform1ui(texturedID[0], 1);
		if(night == true)
		glBindTexture(GL_TEXTURE_2D, textureID6);
		else
		glBindTexture(GL_TEXTURE_2D, textureID10);
		emitmode = 1;
		glUniform1ui(emitmodeID[0], emitmode);
		sphere.drawSphere();
		emitmode = 0;
		model.pop();
	}




	glUseProgram(program[1]);

	//particles
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(-6, 30, 0));
		model.top() = glm::rotate(model.top(), -angle_y, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -angle_z, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale * 40, scale * 40, scale * 40));

																 
		glUniformMatrix4fv(modelID[1], 1, GL_FALSE, &model.top()[0][0]);
		glUniform1ui(colourmodeID[1], colourmode);
		glUniform1f(point_sizeID, point_size);
		glUniformMatrix4fv(viewID[1], 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(projectionID[1], 1, GL_FALSE, &Projection[0][0]);



		point_anim->draw();
		point_anim->animate();
		model.pop();
	}

	//particles
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(6, 30, 0));
		model.top() = glm::rotate(model.top(), -angle_y, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -angle_z, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale * 40, scale * 40, scale * 40));


		glUniformMatrix4fv(modelID[1], 1, GL_FALSE, &model.top()[0][0]);
		glUniform1ui(colourmodeID[1], colourmode);
		glUniform1f(point_sizeID, point_size);
		glUniformMatrix4fv(viewID[1], 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(projectionID[1], 1, GL_FALSE, &Projection[0][0]);



		point_anim->draw();
		point_anim->animate();
		model.pop();
	}
	//particles
	model.push(model.top());
	{
		model.top() = glm::mat4(1.0f);
		model.top() = glm::translate(model.top(), glm::vec3(16, 30, 0));
		model.top() = glm::rotate(model.top(), -angle_y, glm::vec3(0, 1, 0)); 
		model.top() = glm::rotate(model.top(), -angle_z, glm::vec3(0, 0, 1)); 
		model.top() = glm::scale(model.top(), glm::vec3(scale * 40, scale * 40, scale * 40));

												
		glUniformMatrix4fv(modelID[1], 1, GL_FALSE, &model.top()[0][0]);
		glUniform1ui(colourmodeID[1], colourmode);
		glUniform1f(point_sizeID, point_size);
		glUniformMatrix4fv(viewID[1], 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(projectionID[1], 1, GL_FALSE, &Projection[0][0]);



		point_anim->draw();
		point_anim->animate();
		model.pop();
	}

	glDisableVertexAttribArray(0);


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

	if (key == 'D') camera_x += 0.5f;
	if (key == 'A') camera_x -= 0.5f;
	if (key == 'W') camera_y += 0.5f;
	if (key == 'S') camera_y -= 0.5f;
	if (key == 'E') camera_z += 0.5f;
	if (key == 'Q') camera_z -= 0.5f;
	if (key == '1') light_x -= 0.1f;
	if (key == '2') light_x += 0.1f;
	if (key == '3') light_y -= 0.1f;
	if (key == '4') light_y += 0.1f;
	if (key == '5') light_z -= 0.1f;
	if (key == '6') light_z += 0.1f;
	if (key == '7') vx -= 1.f;
	if (key == '8') vx += 1.f;
	if (key == '9') vy -= 1.f;
	if (key == '0') vy += 1.f;
	if (key == 'O') vz -= 1.f;
	if (key == 'P') vz += 1.f;
	if (key == 'K') if (ry < 3.0f) ry += 0.2f;
	if (key == 'L') if (ry > -2.8f) ry -= 0.2f;

	std::cout << "cam_x: " << camera_x << " cam_y: " << camera_y << " cam_z: " << camera_z << std::endl;

	if (key == 'N' && action != GLFW_PRESS) if (night == true) { night = false; } else night = true;


}



/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(1024, 768, "Gregor Whyte - Graphics Assignment 2");;

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





