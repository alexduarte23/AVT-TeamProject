#version 330 core

//VERTEX ATTRIBUTES
in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;
in vec3 inColor;

// INSTANCE ATTRIBUTES
in vec3 inOffset;
in float inSize;

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
out vec4 FragPosLightSpace5;
out vec4 FragPosLightSpace6;

//UNIFORMS
uniform mat4 ModelMatrix;
uniform mat4 campfireLSM1;
uniform mat4 campfireLSM2;
uniform mat4 campfireLSM3;
uniform mat4 campfireLSM4;
uniform mat4 campfireLSM5;
uniform mat4 envLSM;


uniform CameraMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	vec3 realPosition = inPosition * inSize + inOffset;

	exPosition = realPosition;
	exTexcoord = inTexcoord;
	exNormal = transpose(inverse(mat3(ModelMatrix))) * inNormal;
	exColor = inColor;

	vec4 MCPosition = vec4(realPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;

	FragPos = vec3(ModelMatrix * vec4(realPosition, 1.0));
	FragPosLightSpace = campfireLSM1 * vec4(FragPos, 1.0);
	FragPosLightSpace2 = campfireLSM2 * vec4(FragPos, 1.0);
	FragPosLightSpace3 = campfireLSM3 * vec4(FragPos, 1.0);
	FragPosLightSpace4 = campfireLSM4 * vec4(FragPos, 1.0);
	FragPosLightSpace5 = campfireLSM5 * vec4(FragPos, 1.0);
	FragPosLightSpace6 = envLSM * vec4(FragPos, 1.0);
}