// Minimal fragment shader

#version 400


in vec3 N, L, emissive;
in vec4 P;
in vec4 diffuse_albedo;

out vec4 outputColor;

// Global constants (for this fragment shader)
vec3 specular_albedo = vec3(1.0, 0.8, 0.6);
vec3 global_ambient = vec3(0.05, 0.05, 0.05);
int  shininess = 8;

void main()
{
	vec3 normal=normalize(N);
	vec3 lightDirection=normalize(L);
	float distanceToLight = length(lightDirection);	// For attenuation

	// Calculate the diffuse component
	vec3 diffuse = max(dot(normal, lightDirection), 0.0) * diffuse_albedo.xyz;

	// Calculate the specular component using Phong specular reflection
	vec3 V = normalize(-P.xyz);	
	vec3 R = reflect(-lightDirection, normal);
	vec3 specular = pow(max(dot(R, V), 0.0), shininess) * specular_albedo;

	// Calculate the attenuation factor;
	float attenuation_k = 1.0;
    float attenuation = 1.0 / (1.0 + attenuation_k * pow(distanceToLight, 2));


	vec3 ambient = diffuse_albedo.xyz * 0.2;

	// Calculate the output colour, includung attenuation on the diffuse and specular components
	// Note that you may want to exclude the ambient form the attenuation factor so objects
	// are always visible, or include a global ambient

	outputColor =vec4(attenuation*(ambient + diffuse + specular) + emissive + global_ambient, 1.0) ;
}