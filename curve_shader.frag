#version 330 core

out vec4 color;

uniform vec3 RGB;

void main()
{

	color = vec4(RGB, 1.0f);
}
