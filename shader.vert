#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 fragPosition;
out vec3 fragNormal;


uniform mat4 WorldViewProjection;

void main()
{
	fragPosition = position;
	fragNormal = normalize(normal);
	
    gl_Position = WorldViewProjection * vec4(position, 1.0f);
}
