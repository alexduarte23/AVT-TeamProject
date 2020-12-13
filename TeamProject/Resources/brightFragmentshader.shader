#version 330 core

in vec2 exTexcoord;
out vec4 FragmentColor;

uniform sampler2D TexFramebuffer;

void main(void)
{
	vec4 color = texture(TexFramebuffer, exTexcoord);

	//Luma
	float grey = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	FragmentColor = vec4(vec3(grey),  1.0);
}