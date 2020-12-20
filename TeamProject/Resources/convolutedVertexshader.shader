#version 330 core

in vec3 inVertex;
in vec2 inTexcoord;
out vec2 exTexcoord;

uniform mat4 ModelMatrix;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	exTexcoord = inTexcoord;

	vec4 mcVertex = vec4(inVertex, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * mcVertex;
}