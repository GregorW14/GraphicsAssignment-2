// Minimal fragment shader

#version 400


in vec3 N, L, emissive;
in vec4 P;
in vec4 diffuse_albedo;
in vec2 ftexcoord;
flat in uint ftextured;


out vec4 outputColor;

// Global constants (for this fragment shader)
vec3 specular_albedo = vec3(1.0, 0.8, 0.6);
vec3 global_ambient = vec3(0.05, 0.05, 0.05);
int  shininess = 8;

uniform sampler2D tex1;	

void main()
{
	vec3 normal=normalize(N);
	vec3 lightDirection=normalize(L);
	float distanceToLight = length(lightDirection);	// For attenuation

	vec4 texcolour = texture(tex1, ftexcoord);

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

	vec3 texture_diffuse = vec3((ambient + diffuse) * texcolour.xyz);

	if(ftextured == 1)
		outputColor = vec4(attenuation*(texture_diffuse + specular) + emissive + global_ambient, 1.0);
	else
		outputColor =vec4(attenuation*(ambient + diffuse + specular) + emissive + global_ambient, 1.0);
}