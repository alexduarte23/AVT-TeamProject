#version 330 core

uniform vec4 Color;
out vec4 out_Color;


void main(void)
{
	out_Color = Color;
}