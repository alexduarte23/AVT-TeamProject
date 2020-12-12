#version 330 core

//VERTEX ATTRIBUTES
in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;
in vec3 inColor;

//OUTPUTS
out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;
out vec3 exColor;
out vec3 FragPos;

//UNIFORMS
uniform mat4 ModelMatrix;

uniform CameraMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	exPosition = inPosition;
	exTexcoord = inTexcoord;
	exNormal = inNormal;
	exColor = inColor;

	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;

	FragPos = vec3(ModelMatrix * vec4(inPosition, 1.0));
}