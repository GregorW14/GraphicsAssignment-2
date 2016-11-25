#pragma once
/* terrain_object.h
   Example class to show how to create and render a height map
   Iain Martin November 2014
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class terrain_object
{
public:
	terrain_object();
	~terrain_object();

	void calculateNoise(GLfloat freq, GLfloat scale);
	void createTerrain(GLuint xp, GLuint yp, GLfloat xs, GLfloat ys);
	void calculateNormals();

	void createObject();
	void drawObject(int drawmode);

	glm::vec3 *vertices;
	glm::vec3 *normals;
	std::vector<GLushort> elements;
	GLfloat* noise;

	GLuint vbo_mesh_vertices;
	GLuint vbo_mesh_normals;
	GLuint ibo_mesh_elements;
	GLuint attribute_v_coord;
	GLuint attribute_v_normal;

	GLuint xsize;
	GLuint zsize;
	GLfloat width;
	GLfloat height;
	GLuint perlin_octaves;
	GLfloat height_scale;
};

