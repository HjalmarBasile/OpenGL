#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 passLightColor;

uniform mat4 u_View;
uniform mat4 u_ModelView;
uniform mat4 u_MVP;
uniform vec3 u_LightPosition;

uniform vec3 u_AmbientColor;
uniform vec3 u_LightColor;

uniform float u_AmbientStrenght;
uniform float u_DiffuseStrenght;
uniform float u_SpecularStrenght;
uniform float u_SpecularShininess;

void main()
{
	/* Gouraud shading does all the main computations in the vertex shader */

	vec4 posToVec4 = vec4(position, 1.0f);
	gl_Position = u_MVP * posToVec4;

	/* Compute position of the vertex in view space */
	vec4 fragViewSpacePos = u_ModelView * posToVec4;
	/* N.B. fragViewSpacePos.w is 1 by construction */
	vec3 transfFragViewSpacePos = fragViewSpacePos.xyz;

	/* Transform normal so that it is still ortogonal to the surface */
	/* N.B. Beware of ill-conditioned matrices */
	vec3 transfNormal = mat3(transpose(inverse(u_ModelView))) * normal;

	vec3 transfLightViewSpacePos = vec3(u_View * vec4(u_LightPosition, 1.0f));

	/* Below is what we do in fragment shader when implementing Phong model in the standard way */

	/* Compute ambient light component */
	vec3 ambientColor = u_AmbientStrenght * (0.9f * u_AmbientColor + 0.1f * u_LightColor);

	/* Compute diffuse light component */
	vec3 nNormal = normalize(transfNormal);
	vec3 nLightDir = normalize(transfLightViewSpacePos - transfFragViewSpacePos);
	float diffuseIntensity = max(0.0f, dot(nNormal, nLightDir));
	vec3 diffuseColor = u_DiffuseStrenght * diffuseIntensity * u_LightColor;

	/* Compute specular light component */
	vec3 nViewDir = normalize(-transfFragViewSpacePos); /* Viewer is at origin in View space */
	vec3 nReflectDir = reflect(-nLightDir, nNormal);
	float specularIntensity = pow(max(0.0f, dot(nViewDir, nReflectDir)), u_SpecularShininess);
	vec3 specularColor = u_SpecularStrenght * specularIntensity * u_LightColor;

	passLightColor = ambientColor + diffuseColor + specularColor;
}
