#version 330 core

out vec4 color;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;

void main()
{
	color = vec4(u_ObjectColor * u_LightColor, 1.0f);
}
