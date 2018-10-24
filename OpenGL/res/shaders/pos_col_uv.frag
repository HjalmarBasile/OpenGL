#version 330 core

out vec4 color;

in vec3 passColor;
in vec2 passTexCoord;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{
	/* Linear interpolation of the two textures */
	color = mix(texture(u_Texture1, passTexCoord), texture(u_Texture2, passTexCoord), 0.4f);

	/* Let's spice things up! */
	color *= vec4(passColor, 1.0f);
}
