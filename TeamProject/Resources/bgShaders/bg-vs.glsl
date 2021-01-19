#version 330 core

in vec3 inPosition;
in vec3 inColor;

out vec4 exColor;

void main(void)
{
	exColor = vec4(inColor, 1.0);
	
	gl_Position = vec4(inPosition, 1.0);

}