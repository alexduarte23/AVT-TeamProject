#version 330 core

//VERTEX ATTRIBUTES
in vec3 inPosition;
in vec2 inTexcoord;
in vec4 inColor;

//OUTPUTS
out vec2 exTexcoord;
out vec4 exColor;


//UNIFORMS
uniform mat4 ModelMatrix;

uniform CameraMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	exTexcoord = inTexcoord;
	exColor = inColor;
	
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition, 1.0);

}