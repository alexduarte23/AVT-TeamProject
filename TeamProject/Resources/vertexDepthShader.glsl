#version 330 core

//VERTEX ATTRIBUTES
in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;
in vec3 inColor;

uniform mat4 lightSpaceMatrix;
uniform mat4 ModelMatrix;

void main(void)
{
    gl_Position = lightSpaceMatrix * ModelMatrix * vec4(inPosition, 1.0);
} 