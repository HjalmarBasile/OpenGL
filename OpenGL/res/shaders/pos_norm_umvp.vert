#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 passFragViewSpacePos;
out vec3 passNormal;
out vec3 passLightViewSpacePos;

uniform mat4 u_View;
uniform mat4 u_ModelView;
uniform mat4 u_MVP;
uniform vec3 u_LightPosition;

void main()
{
	vec4 posToVec4 = vec4(position, 1.0f);
	gl_Position = u_MVP * posToVec4;

	/* Compute position of the vertex in view space */
	vec4 fragViewSpacePos = u_ModelView * posToVec4;
	/* N.B. fragViewSpacePos.w is 1 by construction */
	passFragViewSpacePos = fragViewSpacePos.xyz;

	/* Transform normal so that it is still ortogonal to the surface */
	/* N.B. Beware of ill-conditioned matrices */
	passNormal = mat3(transpose(inverse(u_ModelView))) * normal;

	passLightViewSpacePos = vec3(u_View * vec4(u_LightPosition, 1.0f));
}
