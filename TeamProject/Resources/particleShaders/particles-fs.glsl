#version 330 core

in vec4 ex_color;
in vec2 texCoord;

uniform sampler2D in_texture;

out vec4 out_color;

void main(void)
{
	vec4 texel = texture(in_texture, texCoord);
	//if(texel.a < 0.2)
	//	discard;

	out_color = texel * ex_color;
}