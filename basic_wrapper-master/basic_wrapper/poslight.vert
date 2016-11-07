// Specify minimum OpenGL version
#version 400

// Define the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;

// This is the output vertex colour sent to the rasterizer
out vec3 N, L, emissive;
out vec4 P;
out vec4 diffuse_albedo; // This is the vertex colour, used to handle the colourmode change


// These are the uniforms that are defined in the application
uniform mat4 model, view, projection;
uniform mat3 normalmatrix;
uniform uint colourmode;
uniform vec4 lightpos;
uniform uint emitmode;



void main()
{
	 emissive = vec3(0);				// Create a vec3(0, 0, 0) for our emmissive light
	vec4 position_h = vec4(position, 1.0);	// Convert the (x,y,z) position to homogeneous coords (x,y,z,w)			
	vec3 light_pos3 = lightpos.xyz;			

	// Define our vectors to calculate diffuse and specular lighting
	mat4 mv_matrix = view * model;		// Calculate the model-view transformation
	P = mv_matrix * position_h;	// Modify the vertex position (x, y, z, w) by the model-view transformation
	N = normalmatrix * normal;		// Modify the normals by the normal-matrix (i.e. to model-view (or eye) coordinates )
	L = light_pos3 - P.xyz;		// Calculate the vector from the light position to the vertex in eye space
		
	//If emitmode is 1 then we enable emmissive lighting
	if (emitmode == 1) 
		emissive = vec3(1.0, 1.0, 0.8); 
	
	// Switch the vertex colour based on the colourmode
	if (colourmode == 1)
		diffuse_albedo = colour;
	else
		diffuse_albedo = vec4(0.2, 0.3, 0.9, 1.0);

	gl_Position = (projection * view * model) * position_h;
}


