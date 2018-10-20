#version 330 core

out vec4 color;

in vec3 passColor;

void main()
{
	color = vec4(passColor, 1.0f);
}
