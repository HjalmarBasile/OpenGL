#version 330 core

out vec4 color;

in vec3 passFragWorldSpacePos;
in vec3 passNormal;

uniform vec3 u_ObjectColor;
uniform vec3 u_AmbientColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

uniform float u_AmbientStrenght;
uniform float u_DiffuseStrenght;
uniform float u_SpecularStrenght;
uniform float u_SpecularShininess;

void main()
{
	/* Compute ambient light component */
	vec3 ambientColor = u_AmbientStrenght * (0.8f * u_AmbientColor + 0.2f * u_LightColor);

	/* Compute diffuse light component */
	vec3 nPassNormal = normalize(passNormal);
	vec3 nLightDir = normalize(u_LightPosition - passFragWorldSpacePos);
	float diffuseIntensity = max(0.0f, dot(nPassNormal, nLightDir));
	vec3 diffuseColor = u_DiffuseStrenght * diffuseIntensity * u_LightColor;

	/* Compute specular light component */
	vec3 nViewDir = normalize(u_ViewPosition - passFragWorldSpacePos);
	vec3 nReflectDir = reflect(-nLightDir, nPassNormal);
	float specularIntensity = pow(max(0.0f, dot(nViewDir, nReflectDir)), u_SpecularShininess);
	vec3 specularColor = u_SpecularStrenght * specularIntensity * u_LightColor;

	color = vec4(u_ObjectColor * (ambientColor + diffuseColor + specularColor), 1.0f);
}
