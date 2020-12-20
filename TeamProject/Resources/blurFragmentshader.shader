#version 330 core

in vec2 exTexcoord;
out vec4 FragmentColor;

uniform sampler2D TexFramebuffer;
const float scaley = 128;

void main(void)
{
	/**/if (fract(exTexcoord.y * scaley) > 0.75)
		discard;
	vec4 color = texture(TexFramebuffer, exTexcoord);
	FragmentColor = color;
	/**/
	//FragmentColor = texture(TexFramebuffer, exTexcoord);
}