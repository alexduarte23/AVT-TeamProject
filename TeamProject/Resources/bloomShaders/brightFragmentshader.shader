#version 330 core

in vec2 exTexcoord; 
out vec4 FragmentColor;

uniform sampler2D TexFramebuffer;

void main(void)
{
	vec4 color = texture(TexFramebuffer, exTexcoord);
    FragmentColor = color;
    
}