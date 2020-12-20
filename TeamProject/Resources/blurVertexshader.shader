#version 330 core

layout(location = 0) in vec2 inVertex;
layout(location = 1) in vec2 inTexcoord;
out vec2 exTexcoord;

void main(void)
{
	exTexcoord = inTexcoord;
	gl_Position = vec4(inVertex, 0.0, 1.0);
}