#pragma once
class Tetrahedron
{
	GLuint tetra_buffer_normals, tetra_buffer_vertices, tetra_buffer_colours, numtetravertices;

public:
	Tetrahedron();
	~Tetrahedron();
	void defineTetrahedron();
	void drawTetrahedron();
	void init();

};

