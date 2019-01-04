#version 330 core

out vec4 color;

in vec3 passFragWorldSpacePos;
in vec3 passNormal;

uniform vec3 u_ObjectColor;
uniform vec3 u_AmbientColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;

void main()
{
	float ambientStrenght = 0.25f;
	vec3 ambientColor = ambientStrenght * (0.8f * u_AmbientColor + 0.2f * u_LightColor);

	// TODO use u_LightColor, u_LightPosition, passFragWorldSpacePos and passNormal for diffuse
	color = vec4(u_ObjectColor * ambientColor, 1.0f);
}
