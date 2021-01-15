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
out vec4 FragPosLightSpace;
out vec4 FragPosLightSpace2;
out vec4 FragPosLightSpace3;
out vec4 FragPosLightSpace4;

//UNIFORMS
uniform mat4 ModelMatrix;
uniform mat4 lightSpaceMatrix;
uniform mat4 lightSpaceMatrix2;
uniform mat4 lightSpaceMatrix3;
uniform mat4 lightSpaceMatrix4;


uniform CameraMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	exPosition = inPosition;
	exTexcoord = inTexcoord;
	exNormal = transpose(inverse(mat3(ModelMatrix))) * inNormal;
	exColor = inColor;

	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;

	FragPos = vec3(ModelMatrix * vec4(inPosition, 1.0));
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	FragPosLightSpace2 = lightSpaceMatrix2 * vec4(FragPos, 1.0);
	FragPosLightSpace3 = lightSpaceMatrix3 * vec4(FragPos, 1.0);
	FragPosLightSpace4 = lightSpaceMatrix4 * vec4(FragPos, 1.0);
}