#version 330 core

out vec4 color;

in vec3 passFragWorldSpacePos;
in vec3 passNormal;

uniform vec3 u_ObjectColor;
uniform vec3 u_AmbientColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

void main()
{
	/* Compute ambient light component */
	float ambientStrenght = 0.8f;
	vec3 ambientColor = ambientStrenght * (0.8f * u_AmbientColor + 0.2f * u_LightColor);

	/* Compute diffuse light component */
	vec3 nPassNormal = normalize(passNormal);
	vec3 nLightDir = normalize(u_LightPosition - passFragWorldSpacePos);
	float diffuseIntensity = max(0.0f, dot(nPassNormal, nLightDir));
	vec3 diffuseColor = diffuseIntensity * u_LightColor;

	color = vec4(u_ObjectColor * (ambientColor + diffuseColor), 1.0f);
}
