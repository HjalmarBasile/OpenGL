#version 330 core

out vec4 color;

in vec3 passLightColor;

uniform vec3 u_ObjectColor;

void main()
{
	/* Gouraud shading does all the main computations in the vertex shader */
	color = vec4(u_ObjectColor * passLightColor, 1.0f);
}
