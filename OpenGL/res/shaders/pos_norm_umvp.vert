#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 passFragWorldSpacePos;
out vec3 passNormal;

uniform mat4 u_Model;
uniform mat4 u_MVP;

void main()
{
	vec4 posToVec4 = vec4(position, 1.0f);
	gl_Position = u_MVP * posToVec4;

	/* Compute position of the vertex in world space */
	vec4 fragWorldSpacePos = u_Model * posToVec4;
	/* N.B. fragWorldSpacePos.w is 1 by construction */
	passFragWorldSpacePos = fragWorldSpacePos.xyz;

	/* Transform normal so that it is still ortogonal to the surface */
	/* N.B. Beware of ill-conditioned matrices */
	passNormal = mat3(transpose(inverse(u_Model))) * normal;
}
