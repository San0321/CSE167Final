#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;

out vec4 color;

uniform vec3 camPosition;
uniform samplerCube skybox;

void main()
{
	vec3 viewDir = normalize(fragPosition - camPosition);
	vec3 R = reflect(-viewDir, fragNormal); 

	color = texture(skybox, R);
}
