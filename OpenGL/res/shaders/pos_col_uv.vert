#version 330 core

layout(location = 0) in vec3 bufPosition;
layout(location = 1) in vec3 bufColor;
layout(location = 2) in vec2 bufTexCoord;

out vec3 passColor;
out vec2 passTexCoord;

void main()
{
	gl_Position = vec4(bufPosition, 1.0f);
	passColor = bufColor;
	passTexCoord = bufTexCoord;
}
