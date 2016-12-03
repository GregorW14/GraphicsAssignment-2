// Minimal fragment shader

#version 400


in vec3 N, L;
in vec4 P;
in vec4 diffuse_albedo;
in vec2 ftexcoord;
flat in uint ftextured;
uniform uint emitmode;


out vec4 outputColor;

// Global constants (for this fragment shader)
vec4 global_ambient = vec4(0.05, 0.05, 0.05, 1.0);
int  shininess = 8;

uniform sampler2D tex1;	

void main()
{
	vec3 normal=normalize(N);
	vec3 lightDirection=normalize(L);
	vec4 emissive = vec4(0);

	vec4 texcolour = texture(tex1, ftexcoord);

	// Calculate the diffuse component
	vec3 diffuse = max(dot(normal, lightDirection), 0.0) * texcolour.xyz;

	// Calculate the specular component using Phong specular reflection
	vec3 V = normalize(-P.xyz);	
	vec3 R = reflect(-lightDirection, normal);

	vec3 ambient = texcolour.xyz * 0.2;

	if(emitmode == 1) emissive = texcolour * vec4(1.0, 1.0, 0.8, 1.0);


	vec3 texture_diffuse = (max(dot(normal, lightDirection), 0.0) * texcolour.xyz);

	outputColor = texcolour;

}